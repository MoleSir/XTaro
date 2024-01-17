#include "verilog.hh"
#include <fstream>
#include <iostream>

namespace xtaro::parse
{
    Verilog::Verilog(int addressWidth, int wordWidth):
        _addressWidth{addressWidth}, _wordWidth{wordWidth}
    {
    }

    void Verilog::writeSRAM(const std::string& filename)
    {
        std::ofstream file{filename};

        this->writeSRAMHeader(file);
        this->writeSRAMSignal(file);
        this->writeSRAMBody(file);
        this->writeSRAMTail(file);

        file.close();
    }

    void Verilog::writeSRAMHeader(std::ofstream& file)
    {
        file << "module sram #(\n";
        file << "    parameter ADDR_WIDTH = " << this->_addressWidth << '\n' << ',';
        file << "    parameter WORD_WIDTH = " << this->_wordWidth << '\n';
        file << ") (\n";
        file << "    input                     clk,\n";
        file << "    input                     csb,\n";
        file << "    input                     we,\n";
        file << "    input [ADDR_WIDTH-1 : 0]  addr,\n";
        file << "    input [WORD_WIDTH-1 : 0]  wdata,\n";
        file << "    output [WORD_WIDTH-1 : 0] rdata\n";
        file << ");\n\n";
    }

    void Verilog::writeSRAMSignal(std::ofstream& file)
    {
        file << "    // ------------------------ Memory ------------------------ //\n";
        file << "    reg [WORD_WIDTH-1 : 0] memory [2**(ADDR_WIDTH)-1 : 0];\n\n";
        file << "    // ------------------------ Register ------------------------ //\n";
        file << "reg                    csb_reg;\n";
        file << "reg                    we_reg;\n";
        file << "reg [ADDR_WIDTH-1 : 0] addr_reg;\n";
        file << "reg [WORD_WIDTH-1 : 0] wdata_reg;\n";
        file << "reg [WORD_WIDTH-1 : 0] rdata_reg;\n\n";
    }

    void Verilog::writeSRAMBody(std::ofstream& file)
    {
        file << "    always @(posedge clk) begin\n";
        file << "        csb_reg   <= csb;\n";
        file << "        we_reg    <= we;\n";
        file << "        addr_reg  <= addr;\n";
        file << "        wdata_reg <= wdata;\n";
        file << "    end\n\n";

        file << "    // ------------------------ Operation ----------------------- //\n";
        file << "    always @(negedge clk) begin : read_operation\n";
        file << "        if (csb_reg == 1'b0 && we_reg == 1'b0) begin\n";
        file << "            rdata_reg = memory[addr_reg];\n";
        file << "        end\n";
        file << "    end\n";
        file << "    assign rdata = rdata_reg\n\n";
        file << "    always @(negedge clk) begin : write_operation\n";
        file << "        if (csb_reg == 1'b0 && we_reg == 1'b1) begin\n";
        file << "            memory[addr_reg] <= wdata_reg;\n";
        file << "        end\n";
        file << "    end\n\n";
    }

    void Verilog::writeSRAMTail(std::ofstream& file)
    {
        file << "endmodule\n";
    }
}