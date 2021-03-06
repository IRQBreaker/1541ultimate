	nios_tester u0 (
		.audio_in_data           (<connected-to-audio_in_data>),           //  audio_in.data
		.audio_in_valid          (<connected-to-audio_in_valid>),          //          .valid
		.audio_in_ready          (<connected-to-audio_in_ready>),          //          .ready
		.audio_out_data          (<connected-to-audio_out_data>),          // audio_out.data
		.audio_out_valid         (<connected-to-audio_out_valid>),         //          .valid
		.audio_out_ready         (<connected-to-audio_out_ready>),         //          .ready
		.dummy_export            (<connected-to-dummy_export>),            //     dummy.export
		.io_ack                  (<connected-to-io_ack>),                  //        io.ack
		.io_rdata                (<connected-to-io_rdata>),                //          .rdata
		.io_read                 (<connected-to-io_read>),                 //          .read
		.io_wdata                (<connected-to-io_wdata>),                //          .wdata
		.io_write                (<connected-to-io_write>),                //          .write
		.io_address              (<connected-to-io_address>),              //          .address
		.io_irq                  (<connected-to-io_irq>),                  //          .irq
		.io_u2p_ack              (<connected-to-io_u2p_ack>),              //    io_u2p.ack
		.io_u2p_rdata            (<connected-to-io_u2p_rdata>),            //          .rdata
		.io_u2p_read             (<connected-to-io_u2p_read>),             //          .read
		.io_u2p_wdata            (<connected-to-io_u2p_wdata>),            //          .wdata
		.io_u2p_write            (<connected-to-io_u2p_write>),            //          .write
		.io_u2p_address          (<connected-to-io_u2p_address>),          //          .address
		.io_u2p_irq              (<connected-to-io_u2p_irq>),              //          .irq
		.jtag0_jtag_tck          (<connected-to-jtag0_jtag_tck>),          //     jtag0.jtag_tck
		.jtag0_jtag_tms          (<connected-to-jtag0_jtag_tms>),          //          .jtag_tms
		.jtag0_jtag_tdi          (<connected-to-jtag0_jtag_tdi>),          //          .jtag_tdi
		.jtag0_jtag_tdo          (<connected-to-jtag0_jtag_tdo>),          //          .jtag_tdo
		.jtag1_jtag_tck          (<connected-to-jtag1_jtag_tck>),          //     jtag1.jtag_tck
		.jtag1_jtag_tms          (<connected-to-jtag1_jtag_tms>),          //          .jtag_tms
		.jtag1_jtag_tdi          (<connected-to-jtag1_jtag_tdi>),          //          .jtag_tdi
		.jtag1_jtag_tdo          (<connected-to-jtag1_jtag_tdo>),          //          .jtag_tdo
		.jtag_in_data            (<connected-to-jtag_in_data>),            //   jtag_in.data
		.jtag_in_valid           (<connected-to-jtag_in_valid>),           //          .valid
		.jtag_in_ready           (<connected-to-jtag_in_ready>),           //          .ready
		.mem_mem_req_address     (<connected-to-mem_mem_req_address>),     //       mem.mem_req_address
		.mem_mem_req_byte_en     (<connected-to-mem_mem_req_byte_en>),     //          .mem_req_byte_en
		.mem_mem_req_read_writen (<connected-to-mem_mem_req_read_writen>), //          .mem_req_read_writen
		.mem_mem_req_request     (<connected-to-mem_mem_req_request>),     //          .mem_req_request
		.mem_mem_req_tag         (<connected-to-mem_mem_req_tag>),         //          .mem_req_tag
		.mem_mem_req_wdata       (<connected-to-mem_mem_req_wdata>),       //          .mem_req_wdata
		.mem_mem_resp_dack_tag   (<connected-to-mem_mem_resp_dack_tag>),   //          .mem_resp_dack_tag
		.mem_mem_resp_data       (<connected-to-mem_mem_resp_data>),       //          .mem_resp_data
		.mem_mem_resp_rack_tag   (<connected-to-mem_mem_resp_rack_tag>),   //          .mem_resp_rack_tag
		.pio_in_port             (<connected-to-pio_in_port>),             //       pio.in_port
		.pio_out_port            (<connected-to-pio_out_port>),            //          .out_port
		.spi_MISO                (<connected-to-spi_MISO>),                //       spi.MISO
		.spi_MOSI                (<connected-to-spi_MOSI>),                //          .MOSI
		.spi_SCLK                (<connected-to-spi_SCLK>),                //          .SCLK
		.spi_SS_n                (<connected-to-spi_SS_n>),                //          .SS_n
		.sys_clock_clk           (<connected-to-sys_clock_clk>),           // sys_clock.clk
		.sys_reset_reset_n       (<connected-to-sys_reset_reset_n>)        // sys_reset.reset_n
	);

