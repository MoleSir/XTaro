#include "bank.hh"

#include <module/bitcellarray.hh>
#include <module/rowdecoder.hh>
#include <module/columnmux.hh>
#include <module/replicabank.hh>
#include <module/prechargearr.hh>
#include <module/writedriverarr.hh>
#include <module/senseamparr.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/format.hh>
#include <util/math.hh>
#include <debug/logger.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string BankArguments::toString() const
    {
        return util::format("aw%d_ww%d", this->addressWidth, this->wordWidth);
    }

    Bank::Bank(String name, BankArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _addressWidth{arguments->addressWidth},
        _wordWidth{arguments->wordWidth},

        _rowSize{util::power(2, arguments->addressWidth)},
        _columnSize{arguments->wordWidth},
        _rowAddressWidth{arguments->addressWidth},
        _columnAddressWidth{0},
        
        _bitcellArray{nullptr},
        _rowDecoder{nullptr},
        _columnMux{nullptr},
        _replicaBank{nullptr},
        _prechargeArray{nullptr},
        _writedriverArray{nullptr},
        _senseampArray{nullptr}
    {
        if (this->_addressWidth < 1 || this->_wordWidth < 1)
        {
            std::string errorMsg {
                util::format(
                    "Bank 's address width '%d' or word width '%d' < 1", 
                    this->_addressWidth, this->_wordWidth
                )
            };

            debug->reportError("Create Bank", errorMsg);
        }

        if (this->_addressWidth > 4)
        {
            while (this->_rowSize > (2 * this->_columnSize))
            {
                this->_rowSize /= 2;
                this->_columnSize *= 2;

                this->_rowAddressWidth -= 1;
                this->_columnAddressWidth += 1;
            }
        }

        logger->debug("Create a 'Bank' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void Bank::createPorts()
    {
        for (int i = 0; i < this->_addressWidth; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("D%d", i), PortType::INPUT);

        this->addPort("wl_en", PortType::INPUT);
        this->addPort("p_en_bar", PortType::INPUT);
        this->addPort("sa_en", PortType::INPUT);
        this->addPort("we_en", PortType::INPUT);

        this->addPort("rbl", PortType::OUTPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("dout%d", i), PortType::OUTPUT);

        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void Bank::createCircuits()
    {
        BitcellArrayArguments bitcellArrayArguments {this->_rowSize, this->_columnSize};
        this->_bitcellArray = factory->create(CircuitType::BITCELL_ARRAY, &bitcellArrayArguments);
        this->_circuits.emplace(this->_bitcellArray);

        RowDecoderArguments rowDecoderArguments {this->_rowAddressWidth};
        this->_rowDecoder = factory->create(CircuitType::ROW_DECODER, &rowDecoderArguments);
        this->_circuits.emplace(this->_rowDecoder);

        if (this->_columnAddressWidth > 0)
        {
            ColumnMuxArguments columnMuxrArguments {this->_columnAddressWidth, this->_wordWidth};
            this->_columnMux = factory->create(CircuitType::COLUMN_MUX, &columnMuxrArguments);
            this->_circuits.emplace(this->_columnMux);
        }

        ReplicaBankArguments rblArguments {this->_rowSize};
        this->_replicaBank = factory->create(CircuitType::REPLICA_BANK, &rblArguments);
        this->_circuits.emplace(this->_replicaBank);

        PrechargeArrayArguments preArguments {this->_wordWidth};
        this->_prechargeArray = factory->create(CircuitType::PRECHARGE_ARRAY, &preArguments);
        this->_circuits.emplace(this->_prechargeArray);

        WriteDriverArrayArguments wriArguments {this->_wordWidth};
        this->_writedriverArray = factory->create(CircuitType::WRITE_DRIVER_ARRAY, &wriArguments);
        this->_circuits.emplace(this->_writedriverArray);

        SenseAmplifierArrayArguments seaArguments {this->_wordWidth};
        this->_senseampArray = factory->create(CircuitType::SENSE_AMPLIFIER_ARRAY, &seaArguments);
        this->_circuits.emplace(this->_senseampArray);
    }

    void Bank::createInstances() 
    {
        // Nets' name
        std::vector<String> blNets{};
        std::vector<String> blgroupNets{};
        std::vector<String> brNets{};
        std::vector<String> brgroupNets{};
        std::vector<String> wlNets{};
        std::vector<String> ANets{};

        for (int i = 0; i < this->_wordWidth; ++i)
        {
            blNets.emplace_back(util::format("bl%d", i));
            brNets.emplace_back(util::format("br%d", i));
        }

        int muxInputSize {util::power(2, this->_columnAddressWidth)};
        for (int muxIdx = 0; muxIdx < this->_wordWidth; ++muxIdx)
        {
            for (int inputIdx = 0; inputIdx < muxInputSize; ++inputIdx)
            {
                blgroupNets.emplace_back(util::format("bl%d_%d", muxIdx, inputIdx));
                brgroupNets.emplace_back(util::format("br%d_%d", muxIdx, inputIdx));
            }
        }
        
        for (int i = 0; i < this->_rowSize; ++i)
            wlNets.emplace_back( util::format("wl%d", i) );
        for (int i = 0; i < this->_addressWidth; ++i)
            ANets.emplace_back( util::format("A%d", i) );

        // Create and Connect bitcell array
        Instance* bitcellArray {this->addInstance("bitcell_arr", this->_bitcellArray)};
        // Bitcell array' ports sequency: bl0 ... bln br0 ... brn ... wl0 ... wlm vdd gnd
        std::vector<String> bitcellarrPorts{};
        for (int i = 0; i < this->_columnSize; ++i)
            bitcellarrPorts.emplace_back(
                this->_columnAddressWidth > 0 ? blgroupNets[i] : blNets[i]
            );
        for (int i = 0; i < this->_columnSize; ++i)
            bitcellarrPorts.emplace_back(
                this->_columnAddressWidth > 0 ? brgroupNets[i] : brNets[i]
            );
        for (int i = 0; i < this->_rowSize; ++i)
            bitcellarrPorts.emplace_back(wlNets[i]);
        bitcellarrPorts.emplace_back("vdd");
        bitcellarrPorts.emplace_back("gnd");
        this->connectWith(bitcellArray, bitcellarrPorts);

        // Create and Connect row decoder
        Instance* rowdecoder {this->addInstance("row_deocder", this->_rowDecoder)};
        // Row decoder's ports sequency: wl_en A0 A1 ... An wl0 wl1 ... wl2^n-1 vdd gnd
        std::vector<String> rowdecoderPorts;
        rowdecoderPorts.emplace_back("wl_en");
        for (int i = 0; i < this->_rowAddressWidth; ++i)
            rowdecoderPorts.emplace_back(ANets[i]);
        for (int i = 0; i < this->_rowSize; ++i)
            rowdecoderPorts.emplace_back(wlNets[i]);
        rowdecoderPorts.emplace_back("vdd");
        rowdecoderPorts.emplace_back("gnd");
        this->connectWith(rowdecoder, rowdecoderPorts);

        if (this->_columnAddressWidth > 0)
        {
            // Create and Connect column mux
            Instance* columnmux {this->addInstance("col_mux", this->_columnMux)};
            
            /*
                - Ports sequency: 
                    -- A0 A1 ... An-1 
                    -- bl0_0 bl0_1 ... 
                    -- br0_0 br0_1 ...
                    -- bl1_0 bl1_1 ...
                    -- br1_0 br1_1 ...
                    -- ...
                    -- blm-1_0 blm-1_1 ...
                    -- brm-1_0 brm-1_1 ...
                    -- bl0 bl1 ...
                    -- br0 br1 ...
                    -- vdd gnd
            */
            std::vector<String> columnmuxPorts{};

            for (int i = 0; i < this->_columnAddressWidth; ++i)
                columnmuxPorts.emplace_back(ANets[i]);
            int blNetIndex {0};
            int brNetIndex {0};

            // For each mux's bl & br
            for (int muxIdx = 0; muxIdx < this->_wordWidth; ++muxIdx)
            {
                for (int inputIdx = 0; inputIdx < muxInputSize; ++inputIdx)
                    columnmuxPorts.emplace_back( blgroupNets[blNetIndex++] );
                for (int inputIdx = 0; inputIdx < muxInputSize; ++inputIdx)
                    columnmuxPorts.emplace_back( brgroupNets[brNetIndex++] );
            }

            // For each mux, it has one bl and br output
            for (int muxIdx = 0; muxIdx < this->_wordWidth; ++muxIdx)
                columnmuxPorts.emplace_back(blNets[muxIdx]);
            for (int muxIdx = 0; muxIdx < this->_wordWidth; ++muxIdx)
                columnmuxPorts.emplace_back(brNets[muxIdx]);

            columnmuxPorts.emplace_back("vdd");
            columnmuxPorts.emplace_back("gnd");
            this->connectWith(columnmux, columnmuxPorts);
        }

        // Create and Connect replica bank
        Instance* replica {this->addInstance("raplica_bank", this->_replicaBank)};
        // wl_en p_en_bar sa_en we_en rbl vdd gnd
        this->connectWith(replica, {"wl_en", "p_en_bar", "sa_en", "we_en", "rbl", "vdd", "gnd"});

        // Create and Connect precharge array
        Instance* precharge {this->addInstance("precharge_array", this->_prechargeArray)};
        // bl0 bl1 ... br0 br1 ... p_en_bar vdd gnd
        std::vector<String> prechargePorts{};
        for (int i = 0; i < this->_wordWidth; ++i)
            prechargePorts.emplace_back(blNets[i]);
        for (int i = 0; i < this->_wordWidth; ++i)
            prechargePorts.emplace_back(brNets[i]);
        prechargePorts.emplace_back("p_en_bar");
        prechargePorts.emplace_back("vdd");
        prechargePorts.emplace_back("gnd");
        this->connectWith(precharge, prechargePorts);

        // Create and Connect sensa amp array
        Instance* senseamp {this->addInstance("sense_amp_array", this->_senseampArray)};
        // bl0 bl1 ... br0 br1 ... dout0 dout1 ... sa_en vdd gnd
        std::vector<String> senseampPorts{};
        for (int i = 0; i < this->_wordWidth; ++i)
            senseampPorts.emplace_back(blNets[i]);
        for (int i = 0; i < this->_wordWidth; ++i)
            senseampPorts.emplace_back(brNets[i]);
        for (int i = 0; i < this->_wordWidth; ++i)
            senseampPorts.emplace_back(util::format("dout%d", i));
        senseampPorts.emplace_back("sa_en");
        senseampPorts.emplace_back("vdd");
        senseampPorts.emplace_back("gnd");
        this->connectWith(senseamp, senseampPorts);

        // Create and Connect write driver array
        Instance* writedriver {this->addInstance("write_driver_array", this->_writedriverArray)};
        // bin0 bin1 ... bl0 bl1 ... br0 br1 ... we_en vdd gnd
        std::vector<String> writedriverPorts{};
        for (int i = 0; i < this->_wordWidth; ++i)
            writedriverPorts.emplace_back(util::format("din%d", i));
        for (int i = 0; i < this->_wordWidth; ++i)
            writedriverPorts.emplace_back(blNets[i]);
        for (int i = 0; i < this->_wordWidth; ++i)
            writedriverPorts.emplace_back(brNets[i]);
        writedriverPorts.emplace_back("we_en");
        writedriverPorts.emplace_back("vdd");
        writedriverPorts.emplace_back("gnd");
        this->connectWith(writedriver, writedriverPorts);
    }
}