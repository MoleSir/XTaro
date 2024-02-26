#include "liberty.hh"
#include <module/sram.hh>
#include <character/charresult.hh>
#include <util/format.hh>
#include <config/option.hh>

#include <fstream>
#include <string>
#include <cstdarg>
#include <sstream>
#include <vector>

namespace xtaro::parse
{
    Liberty::Liberty(
            circuit::SRAM* sram,
            character::CharacterResult* results,
            PVT pvt) :
        _sram{sram},
        _results{results},
        _pvt{std::move(pvt)},
        _libfile{}
    {
        // Create libfile name
        std::string filename {util::format(
            "%s_%s_%f_%f.lib",
            this->_sram->name().data(),
            this->_pvt.process.c_str(),
            this->_pvt.voltage,
            this->_pvt.temperature
        )};
        std::string filepath {util::format(
            "%s/%s",
            option->libFolderPath.c_str(),
            filename.c_str()
        )};

        this->_libfile.open(filepath);
    }

    void Liberty::write()
    {
        this->writeLibrary();
    }

    void Liberty::writeLibrary()
    {
        this->writeLine(0, "library (%s_lib) {", this->_sram->name().data());
        this->writeLine(1, "delay_model : \"table_lookup\";");

        this->writeUnits();
        this->writeDefaults();
        this->writeLUTTemplates();
        this->writeBus();

        this->writeCell();

        this->writeLine(0, '}'); // library
    }

    void Liberty::writeCell()
    {
        this->writeLine(1, "cell (%s) {", this->_sram->name().data());
        
        this->writeLine(2, "memory() {");
        this->writeLine(3, "type : ram;");
        this->writeLine(3, "address_width : %d;", this->_sram->addressWidth());
        this->writeLine(3, "word_width : %d;", this->_sram->wordWidth());
        this->writeLine(2, "}"); // memory
        this->writeLine(2, "interface_timing : true;");
        this->writeLine(2, "dont_use  : true;");
        this->writeLine(2, "map_only   : true;");
        this->writeLine(2, "dont_touch : true;");

        this->writePGPin();

        this->writeWordBus();

        this->writeAdderBus();

        this->writeControlPins();

        this->writeLine(1, '}'); // cell
    }

    void Liberty::writeUnits()
    {
        this->writeLine(1, "time_unit : \"1ns\";");
        this->writeLine(1, "voltage_unit : \"1V\"");
        this->writeLine(1, "current_unit : \"1mA\";");
        this->writeLine(1, "resistance_unit : \"1kohm\";");
        this->writeLine(1, "capacitive_load_unit(1, pF);");
        this->writeLine(1, "leakage_power_unit : \"1mW\";");
        this->writeLine(1, "pulling_resistance_unit :\"1kohm\";");
        this->writeLine(1, "operating_conditions(OC) {");
        this->writeLine(2, "process : %s;", this->_pvt.process.c_str());
        this->writeLine(2, "voltage : %f", this->_pvt.voltage);
        this->writeLine(2, "temperature : %f", this->_pvt.temperature);
        this->writeLine(1, "}");
        this->writeLine(0, '\n');
    }

    void Liberty::writeDefaults()
    {
        this->writeLine(1, "input_threshold_pct_fall      : 50.0;");
        this->writeLine(1, "output_threshold_pct_fall     : 50.0;");
        this->writeLine(1, "input_threshold_pct_rise      : 50.0;");
        this->writeLine(1, "output_threshold_pct_rise     : 50.0;");
        this->writeLine(1, "slew_lower_threshold_pct_fall : 10.0;");
        this->writeLine(1, "slew_upper_threshold_pct_fall : 90.0;");
        this->writeLine(1, "slew_lower_threshold_pct_rise : 10.0;");
        this->writeLine(1, "slew_upper_threshold_pct_rise : 90.0;");
        
        // TODO nom
        this->writeLine(1, "default_cell_leakage_power    : 0.0;");
        this->writeLine(1, "default_leakage_power_density : 0.0;");
        this->writeLine(1, "default_input_pin_cap         : 1.0;");
        this->writeLine(1, "default_inout_pin_cap         : 1.0;");
        this->writeLine(1, "default_output_pin_cap        : 0.0;");
        this->writeLine(1, "default_max_transition        : 0.5;");
        this->writeLine(1, "default_fanout_load           : 1.0;");
        this->writeLine(1, "default_max_fanout            : 4.0;");
        this->writeLine(1, "default_connection_class      : universal;");

        this->writeLine(1, "voltage_map (vdd, %f);", this->_pvt.voltage);
        this->writeLine(1, "voltage_map (gnd, 0);");

        this->writeLine(1, "default_operating_conditions : OC;");

        this->writeEntry();
    }

    void Liberty::writeLUTTemplates()
    {
        this->writeLine(1, "lu_table_template(CELL_TABLE) {");
        this->writeLine(2, "variable_1 : input_net_transition;");
        this->writeLine(2, "variable_2 : total_output_net_capacitance;");
        this->writeIndex(1, this->_results->slews);
        this->writeIndex(2, this->_results->loads);
        this->writeLine(1, '}');
        
        this->writeEntry();

        this->writeLine(1, "lu_table_template(CONSTRAINT_TABLE) {");
        this->writeLine(2, "variable_1 : related_pin_transition;");
        this->writeLine(2, "variable_2 : constrained_pin_transition;");
        this->writeIndex(1, this->_results->slews);
        this->writeIndex(2, this->_results->slews);
        this->writeLine(1, '}');

        this->writeEntry();
    }

    void Liberty::writeBus()
    {
        this->writeLine(1, "type (data) {");
        this->writeLine(2, "base_type : array;");
        this->writeLine(2, "data_type : bit;");
        this->writeLine(2, "bit_width : %d;", this->_sram->wordWidth());
        this->writeLine(2, "bit_from : %d;", this->_sram->wordWidth() - 1);
        this->writeLine(2, "bit_to : 0;");
        this->writeLine(1, '}');
        this->writeEntry();

        this->writeLine(1, "type (addr) {");
        this->writeLine(2, "base_type : array;");
        this->writeLine(2, "data_type : bit;");
        this->writeLine(2, "bit_width : %d;", this->_sram->addressWidth());
        this->writeLine(2, "bit_from : %d;", this->_sram->addressWidth() - 1);
        this->writeLine(2, "bit_to : 0;");
        this->writeLine(1, '}');
        this->writeEntry();
    }

    void Liberty::writePGPin()
    {
        this->writeLine(2, "pg_pin(vdd) {");
        this->writeLine(3, "voltage_name : VDD;");
        this->writeLine(3, "pg_type : primary_power;");
        this->writeLine(2, '}');
        this->writeEntry();

        this->writeLine(2, "pg_pin(gnd) {");
        this->writeLine(3, "voltage_name : gnd;");
        this->writeLine(3, "pg_type : primary_ground;");
        this->writeLine(2, '}');
        this->writeEntry();
    }

    void Liberty::writeWordBus()
    {
        this->writeWordBusInput();
        this->writeWordBusOutput();
    }

    void Liberty::writeWordBusInput()
    {
        this->writeLine(2, "bus(din) {");
        this->writeLine(3, "bus_type  : data;");
        this->writeLine(3, "direction  : input;");
        // this->writeLine(2, "max_capacitance : %d;", this->maxLoad());
        // this->writeLine(2, "min_capacitance : %d;", this->minLoad());
        // this->writeLine(2, "capacitance : %f", tech->spice["dff_in_cap"]);
        this->writeLine(3, "memory_write() {");
        this->writeLine(4, "address : addr");
        this->writeLine(4, "clocked_on  : clk");
        this->writeLine(3, '}'); // memory_write
        this->writeLine(3, "pin(din[%d:0]) {", this->_sram->wordWidth() - 1);
        // TODO setup hold
        this->writeLine(3, '}'); // pin
        this->writeLine(2, '}'); // bus

        this->writeEntry();
    }

    void Liberty::writeWordBusOutput()
    {
        this->writeLine(2, "bus(dout) {");
        this->writeLine(3, "bus_type  : data;");
        this->writeLine(3, "direction  : output;");
        this->writeLine(3, "max_capacitance : ");
        this->writeLine(3, "min_capacitance : ");
        this->writeLine(3, "memory_read() {");
        this->writeLine(4, "address : addr");
        this->writeLine(3, '}'); // memory_read()
        this->writeLine(3, "pin(dout[%d:0]) {", this->_sram->wordWidth() - 1);
        this->writeLine(3, '}'); // pin
        this->writeLine(2, '}'); // bus
    }

    void Liberty::writeAdderBus()
    {
        this->writeLine(2, "bus(addr) {");
        this->writeLine(3, "bus_type  : addr;");
        this->writeLine(3, "direction  : input;");
        // this->writeLine(2, "max_capacitance : ");
        // this->writeLine(2, "min_capacitance : ");
        this->writeLine(3, "memory_read() {");
        this->writeLine(3, "address : addr");
        this->writeLine(3, '}'); // memory_read()
        this->writeLine(3, "pin(dout[%d:0]) {", this->_sram->wordWidth() - 1);
        this->writeLine(3, '}'); // pin
        this->writeLine(2, '}'); // bus
    }

    void Liberty::writeControlPins()
    {
        this->writeLine(2, "pin(csb) {");
        this->writeLine(3, "direction  : input;");
        this->writeLine(2, '}'); // pin

        this->writeLine(2, "pin(we) {");
        this->writeLine(3, "direction  : input;");
        this->writeLine(2, '}'); // pin

        this->writeLine(2, "pin(clk) {");
        this->writeLine(3, "direction  : input;");
        this->writeLine(2, '}'); // pin
    }

    void Liberty::writeIndex(int number, const std::vector<double>& values)
    {
        std::stringstream ss {};

        ss << "index_" << number << '(' << '"';
        bool firstFlag {true};
        for (double v : values)
        {
            if (!firstFlag) ss << ',' << ' ';
            else firstFlag = false;
            ss << v;
        }
        ss << '"' << ')';

        this->writeLine(2, ss.str());   
    }

    void Liberty::writeLine(int indent, const std::string& content)
    {
        while (indent--) this->_libfile << "    ";
        this->_libfile << content << '\n';
    }

    void Liberty::writeLine(int indent, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        std::string content {util::vformat(fmt, args)};
        va_end(args);

        this->writeLine(indent, content);
    }

    void Liberty::writeLine(int indent, char ch)
    {
        while (indent--) this->_libfile << "    ";
        this->_libfile << ch << '\n';
    }

    void Liberty::writeEntry()
    {
        this->_libfile << '\n';
    }
}