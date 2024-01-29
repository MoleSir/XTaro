#include "interactivextaro.hh"

#include <factory/circuitfactory.hh>

#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <character/function.hh>

#include <debug/exception.hh>
#include <config/option.hh>
#include <config/tech.hh>
#include <util/format.hh>
#include <util/sys.hh>
#include <debug/debug.hh>

#include <iostream>
#include <functional>
#include <cstring>

namespace xtaro 
{

    void InteractiveXTaro::loadOption()
    {
        if (this->_arguments.size() == 0)
        {
            debug->error("Command 'load_option' needs one argument as option file~");
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

            this->_loadOptionSucc = true;
        }
        catch (const std::exception& err)
        {
            debug->error("'load_option' failed~");
            // TODO: Reset option and tech
            return;
        }
    }

    void InteractiveXTaro::catOption()
    {
        if (!this->checkOption()) return;

        std::cout << "word width:    " << option->wordWidth << std::endl;
        std::cout << "address width: " << option->addressWidth << std::endl;

        std::cout << "tech path:     " << option->techPath << std::endl;
        std::cout << "output path:   " << option->outputPath << std::endl;
    }

    void InteractiveXTaro::compile()
    {
        if (!this->checkOption()) return;
 
        try
        {
            debug->info("Generate SRAM circuit...");
            circuit::SRAMArguments argument {option->addressWidth, option->wordWidth};
            this->_sram = std::make_unique<circuit::SRAM>(option->sramName, &argument);
            debug->info("Generate SRAM circuit successfully!");
            
            this->_compileSucc = true;
        }
        catch (const std::exception& err) 
        {
            debug->error("'compile' failed~");
            circuit::circuitFactory->reset();
            this->_sram.reset();
            return;
        }
    }

    void InteractiveXTaro::save()
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

    void InteractiveXTaro::saveSpice()
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

    void InteractiveXTaro::saveVerilog()
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

    void InteractiveXTaro::functionTest()
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

    void InteractiveXTaro::exit()
    {
        throw InteractiveExit{};
    }

    void InteractiveXTaro::clear()
    {
    #ifdef __linux__
        util::execute("clear");
    #elif  _WIN32
        util::execute("cls");
    #elif __APPLE__
        util::execute("clear");
    #else
        static_assert(false);
    #endif
    }

    void InteractiveXTaro::setDebugLevel()
    {
        if (this->_arguments.size() != 1)
        {
            debug->error("Command 'set_debug_level' needs one argument as new level~");
            return;
        }

        const std::string& level {this->_arguments[0]};
        if (std::strncmp(level.c_str(), "DEBUG", 5) == 0)
            debug->setLevel(DebugLevel::DEBUG);
        else if (std::strncmp(level.c_str(), "INFO", 4) == 0)
            debug->setLevel(DebugLevel::INFO);
        else if (std::strncmp(level.c_str(), "WARNING", 7) == 0)
            debug->setLevel(DebugLevel::WARNING);
        else if (std::strncmp(level.c_str(), "ERROR", 5) == 0)
            debug->setLevel(DebugLevel::ERROR);
        else if (std::strncmp(level.c_str(), "FATAL", 5) == 0)
            debug->setLevel(DebugLevel::FATAL);
        else
        {
            debug->error("'%s' is not a debug level~", level.c_str());
        }
    }
}
