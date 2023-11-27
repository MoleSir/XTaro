# `openram.report_status()`

根据之前的配置，输出一些 openram 的状态：

````python
def report_status():
    """
    Check for valid arguments and report the
    info about the SRAM being generated
    """
    global OPTS

    # Check if all arguments are integers for bits, size, banks
    if type(OPTS.word_size) != int:
        debug.error("{0} is not an integer in config file.".format(OPTS.word_size))
    if type(OPTS.num_words) != int:
        debug.error("{0} is not an integer in config file.".format(OPTS.sram_size))
    if type(OPTS.write_size) is not int and OPTS.write_size is not None:
        debug.error("{0} is not an integer in config file.".format(OPTS.write_size))

    # If a write mask is specified by the user, the mask write size should be the same as
    # the word size so that an entire word is written at once.
    if OPTS.write_size is not None and OPTS.write_size != OPTS.word_size:
        if (OPTS.word_size % OPTS.write_size != 0):
            debug.error("Write size needs to be an integer multiple of word size.")
        # If write size is more than half of the word size,
        # then it doesn't need a write mask. It would be writing
        # the whole word.
        if (OPTS.write_size < 1 or OPTS.write_size > OPTS.word_size / 2):
            debug.error("Write size needs to be between 1 bit and {0} bits.".format(int(OPTS.word_size / 2)))

    if not OPTS.tech_name:
        debug.error("Tech name must be specified in config file.")

    debug.print_raw("Technology: {0}".format(OPTS.tech_name))
    total_size = OPTS.word_size*OPTS.num_words*OPTS.num_banks
    debug.print_raw("Total size: {} bits".format(total_size))
    if total_size >= 2**14 and not OPTS.analytical_delay:
        debug.warning("Characterizing large memories ({0}) will have a large run-time.".format(total_size))
    debug.print_raw("Word size: {0}\nWords: {1}\nBanks: {2}".format(OPTS.word_size,
                                                                    OPTS.num_words,
                                                                    OPTS.num_banks))
    if (OPTS.write_size != OPTS.word_size):
        debug.print_raw("Write size: {}".format(OPTS.write_size))
    debug.print_raw("RW ports: {0}\nR-only ports: {1}\nW-only ports: {2}".format(OPTS.num_rw_ports,
                                                                                 OPTS.num_r_ports,
                                                                                 OPTS.num_w_ports))

    if OPTS.netlist_only:
        debug.print_raw("Netlist only mode (no physical design is being done, netlist_only=False to disable).")

    if not OPTS.route_supplies:
        debug.print_raw("Design supply routing skipped. Supplies will have multiple must-connect pins. (route_supplies=True to enable supply routing).")

    if not OPTS.inline_lvsdrc:
        debug.print_raw("DRC/LVS/PEX is only run on the top-level design to save run-time (inline_lvsdrc=True to do inline checking).")

    if not OPTS.check_lvsdrc:
        debug.print_raw("DRC/LVS/PEX is disabled (check_lvsdrc=True to enable).")

    if OPTS.analytical_delay:
        debug.print_raw("Characterization is disabled (using analytical delay models) (analytical_delay=False to simulate).")
    else:
        if OPTS.spice_name != "":
            debug.print_raw("Performing simulation-based characterization with {}".format(OPTS.spice_name))
        if OPTS.trim_netlist:
            debug.print_raw("Trimming netlist to speed up characterization (trim_netlist=False to disable).")
    if OPTS.nominal_corner_only:
        debug.print_raw("Only generating nominal corner timing.")
````

