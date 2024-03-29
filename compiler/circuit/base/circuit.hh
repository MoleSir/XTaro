#pragma once

#include "circuitenum.hh"
#include "instance.hh"
#include "net.hh"
#include "port.hh"

#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <string_view>

namespace xtaro::circuit
{

    struct CircuitArguments 
    {
        virtual ~CircuitArguments() noexcept = default;
        virtual std::string toString() const = 0; 
    };
    
    /*
        A base abstract class for all circuit. 
        Each 'Circuit' object stands for a '.SUBCKT' in spice file.
    */
    class Circuit
    {
    public:
        /*
            Circuit construction.
            - name: This circuit's name. In spice, it is the name of a '.SUBCKT';
            - type: The spice device type of this circuit. e.g, MOS, Diode, Capacitance, Resistance, SUBCKT;
            - spicefile: The spice description file of this circuit if it exits. (Default value: "").
        */
        Circuit(const std::string_view& name, DeviceType type, const std::string& spicefile = "");
        Circuit(const Circuit&) = delete;
        Circuit(Circuit&&) = delete;
        Circuit& operator = (const Circuit&) = delete;
        Circuit& operator = (Circuit&&) = delete;
        virtual ~Circuit() noexcept;

    public:
        /*
            Add an port to this circuit.
            - portName: the port's name;
            - portType: the port's type.
        */
        void addPort(const std::string_view& portName, PortType portType);

        /*
            Add ports to this circuit.
            - portName: the ports' name;
            - portType: the ports' type.
        */
        void addPorts(const std::vector<std::string_view>& portsName, PortType portType);

        /*
            Return all ports's name as a std::vector<const char*>
        */
        std::vector<std::string> portsName() const;

        /*
            Copy all ports in this circuit, return as a std::vector<Port*>.
            This is a deep copy, the method will take the datas in this ports,
            and create some new ports object.
            This method mainly used in creating a Instance object.
        */
        std::vector<Port*> copyPort() const;

        /*
            Add an Instance object to this circuit.
            - instanceName: the Instance object's name. In spice, it's the name after devide type keyword. 
              e.g, "Xinv A Z vdd gnd inv";
            - circuit: The pointer to the target Circuit object of the instance.
        */
        Instance* addInstance(const std::string_view& instanceName, Circuit* circuit);

        /*
            Connect the 'nets' with 'instance'. The method will create each Net object corresponding the name in 'nets'.
            And connect the Net objects to the Port objects in 'instance' by sequently.
            - instance: the target instance this circuit want to connect with. 
              Please ensure the instance in this circuit, the mehtod does not check;
            - nets: vector of nets' name.
        */
        void connectWith(Instance* instance, const std::vector<std::string_view>& nets);

        /*
            Add an Instance object to this circuit. And connet it with nets.
            This method equal to call 'addInstance(instanceName, citcuit)' and 'connectWith(instance, nets)'.
        */
        Instance* addInstance(const std::string_view& instanceName, Circuit* circuit, const std::vector<std::string_view>& nets);

        /*
            Add a Cricuit object to this circuit.
        */
        Circuit* addCircuit(const std::string_view& circuitTypeName, CircuitArguments* arguments, const std::string_view& circuitName = "");

        /*
            Write this circuit's spcice description into 'filename'.
        */
        void writeSpice(const std::string& filename);
        
        /*
            Create a new circuit:
            - Add ports
            - Create Sub Circuits
            - Create and connect instance
        */
        void createNetlist();

    protected:
        /*
            Create Net objects in this circuit whose name in 'netsName'. 
            If netname is already been created, just return it, or create a new Net object with net name.
            And finally, return Net objects as a vector.
            - netsName: the names vector of Net objects.
        */
        std::vector<Net*> createNets(const std::vector<std::string_view>& netsName);

    protected:
        void doWriteSpice(std::ofstream& file, std::set<Circuit*>& visited);

        virtual void createPorts() = 0;
        virtual void createCircuits() = 0;
        virtual void createInstances() = 0;
        
    public:
        virtual std::string argumentsList() const 
        { return ""; }

    public:
        const std::string_view& name() const noexcept
        { return this->_name; }

        DeviceType type() const noexcept
        { return this->_type; }
        
        const std::vector<Port*>& ports() const noexcept
        { return this->_ports; } 

        const std::set<Circuit*>& circuits() const noexcept
        { return this->_circuits; }

        const std::vector<Instance*>& instances() const noexcept
        { return this->_instances; }

    protected:
        std::string_view _name;
        DeviceType  _type;

        std::set<Circuit*> _circuits;
        std::vector<Instance*> _instances;
        std::vector<Port*> _ports;
        std::map<std::string_view, Net*> _nets;

        bool _isMetaCircuit;
        std::string _metaSpice;
    };

}