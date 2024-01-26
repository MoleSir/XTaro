#include "interpreterxtaro.hh"

#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <character/function.hh>

#include <debug/exception.hh>
#include <config/option.hh>
#include <config/tech.hh>
#include <util/format.hh>
#include <debug/debug.hh>

#include <iostream>
#include <functional>
#include <cstring>

namespace xtaro 
{

    void InterpreterXTaro::loadOption()
    {
        if (this->_arguments.size() == 0)
        {
            debug->error("Command 'load_option' needs one argument as option file");
            return;
        }

        // Frist arguments is option file
        try
        {
            debug->info("Load Option...");
            const std::string& optionFile {this->_arguments[0]};
            option->load(optionFile);
            debug->info("Load Option successfully!");

            debug->info("Load Tech...");
            tech->load();
            debug->info("Load Tech successfully!");
        }
        catch (const std::exception& err)
        {
            debug->error("'load_option' failed~");
            return;
        }

        this->_loadOptionSucc = true;
    }

    void InterpreterXTaro::catOption()
    {
        if (!this->checkOption()) return;

        std::cout << "word width:    " << option->wordWidth << std::endl;
        std::cout << "address width: " << option->addressWidth << std::endl;
    }

    void InterpreterXTaro::compile()
    {
        if (!this->checkOption()) return;
 
        try
        {
            debug->info("Generate SRAM circuit...");
            circuit::SRAMArguments argument {option->addressWidth, option->wordWidth};
            this->_sram = std::make_unique<circuit::SRAM>(option->sramName, &argument);
            debug->info("Generate SRAM circuit successfully!");
        }
        catch (const std::exception& err) 
        {
            debug->error("'compile' failed~");
            return;
        }
        
        this->_compileSucc = true;
    }

    void InterpreterXTaro::save()
    {
        if (!this->checkCompile()) return;

        try
        {
            // Arguments
            bool spiceFile {false};
            bool verilogFile {false};
            for (const std::string& argument : this->_arguments)
            {
                if (std::strncmp(argument.c_str(), "--spice", 7) == 0)
                    spiceFile = true;
                else if (std::strncmp(argument.c_str(), "--verilog", 9) == 0)
                    verilogFile = true;
                else
                    debug->error(util::format("Unkown argument '%s' in 'save' command~", argument.c_str()));
            }

            if (spiceFile)
            {
                debug->info("Write spice file...");
                this->_sram->writeSpice(option->spicePath);
                debug->info("Write spice file successfully!");
            }

            if (verilogFile)
            {
                debug->info("Write verilog file...");
                parse::Verilog verilog {option->addressWidth, option->wordWidth};
                verilog.writeSRAM(option->verilogPath);
                debug->info("Write verilog file successfully!");
            }
        }
        catch (const std::exception& err) 
        {
            debug->error("'save' failed~");
            return;
        }
    }

    void InterpreterXTaro::saveSpice()
    {
        if (!this->checkCompile()) return;

        try
        {
            debug->info("Write spice file...");
            this->_sram->writeSpice(option->spicePath);
            debug->info("Write spice file successfully!");
        }
        catch (const std::exception& err)
        {
            debug->error("'save_spice' failed~");
            return;
        }
    }

    void InterpreterXTaro::saveVerilog()
    {
        if (!this->checkCompile()) return;

        try
        {
            debug->info("Write verilog file...");
            parse::Verilog verilog {option->addressWidth, option->wordWidth};
            verilog.writeSRAM(option->verilogPath);
            debug->info("Write verilog file successfully!");
        }
        catch (const std::exception& err)
        {
            debug->error("'save_verilog' failed~");
            return;
        }
    }

    void InterpreterXTaro::functionTest()
    {
        if (!this->checkCompile()) return;

        try
        {
            // Arguments
            if (this->_arguments.size() < 3)
            {
                debug->error("Command arguments are too less~\nUsage: 'function_test process voltage temp [times]'");
                return;
            }

            const std::string& process {this->_arguments[0]};
            double voltage{0.0};
            double temperature{0.0};

            try { voltage     = std::stod(this->_arguments[1]); } 
            catch (const std::invalid_argument& err) { 
                debug->error(err.what()); 
                return;
            }
            
            try { temperature = std::stod(this->_arguments[2]); } 
            catch (const std::invalid_argument& err) {
                debug->error("'%s' is not a double value~", this->_arguments[1].c_str());
                return;
            }

            PVT pvt {process, voltage, temperature};
            debug->info(
                "Functional test in 'PVT(%s, %f, %f)'...", 
                pvt.process.c_str(), pvt.voltage, pvt.temperature
            );
            character::FunctionSimulator function {this->_sram.get(), std::move(pvt)};
            bool result {function.randomTest()};
            if (result)
                debug->info("Functional test pass!");
            else
                debug->info("Functional test failed~");
        }
        catch (const std::exception& err)
        {
            debug->error("'function_test' failed~");
            return;
        }   
    }

    void InterpreterXTaro::exit()
    {
        throw InterpreterExit{};
    }

    void InterpreterXTaro::clear()
    {
        std::system("clear");
    }

}
