#include "systemc.h"
#include "RAM.cpp"
#include <scv.h>

class Pkt_constraint : virtual public scv_constraint_base
{
	public:
	scv_smart_ptr<sc_uint<ADDR_WIDTH> > sAddr;
	scv_smart_ptr<sc_uint<DATA_WIDTH> > sInData;
	scv_smart_ptr<sc_uint<DATA_WIDTH> > sOutData;

	SCV_CONSTRAINT_CTOR(Pkt_constraint) {
		// define constraints
		SCV_CONSTRAINT( sAddr() < (1<<ADDR_WIDTH));

		SCV_CONSTRAINT( sInData() < (1<<DATA_WIDTH));
		}
};

int sc_main (int argc, char* argv[]) {

	// Declare Input/Output Signals
  	sc_signal < sc_uint<ADDR_WIDTH> > 	tAddr ;
  	sc_signal < bool > 			tbWE;
  	sc_signal < bool > 			tbCE;
  	sc_signal < sc_uint<DATA_WIDTH> > 	tInData;
  	sc_signal < sc_uint<DATA_WIDTH> > 	tOutData;
  	
	int i = 0;
	
	// Connect the DUT(Design Under Test)
	SRAM RAM_01("SIMULATION_RAM");
		RAM_01.InData(tInData);
        	RAM_01.Addr(tAddr);
		RAM_01.bCE(tbCE);
		RAM_01.bWE(tbWE);
		RAM_01.OutData(tOutData);

	// Open VCD(Value Change Dump) file
	sc_trace_file *wf = sc_create_vcd_trace_file("VCD_RAM");

	// Dump the desired signals
	sc_trace(wf, tInData, "strInData");
        sc_trace(wf, tAddr, "strAddr");
	sc_trace(wf, tbCE, "strbCE");
	sc_trace(wf, tbWE, "strbWE");
	sc_trace(wf, tOutData, "strOutData");

	// Initialize all variables
	tbCE.write(1);
	tbWE.write(1);
	sc_start(5);

	Pkt_constraint cPkt("Pkt_Test");

	typedef pair < sc_uint<ADDR_WIDTH>,sc_uint<ADDR_WIDTH> > addr_range;
	scv_bag<addr_range> addr_list;
	addr_list.add(addr_range(1,9),100);

	typedef pair < sc_uint<DATA_WIDTH>,sc_uint<DATA_WIDTH> > data_range;
	scv_bag<data_range> data_list1;
	data_list1.add(data_range(80,(1<<DATA_WIDTH)-1),100);


        // Verification I
        cout << "------------------- Beginning of Verification I" << endl;
        for(i=0; i<5; i++){
          // Set writing mode for RAM
	  		tbCE.write(0);
			tbWE.write(0);
          // Generate values for tInData & tAddr using "cPkt"
			cPkt.sAddr->set_mode(addr_list);
			cPkt.sInData->set_mode(data_list1);
			cPkt.next();
			tAddr.write(cPkt.sAddr->read());
			tInData.write(cPkt.sInData->read());
			sc_start(1);
          // Set reading mode for RAM
	  		tbCE.write(0);
			tbWE.write(1);
			sc_start(1);
          // Data read from tOutData, save to cPkt.sOutData
		  	*cPkt.sOutData = tOutData.read();
          // Print statistics
	  		cPkt.print();
	  		sc_start(1);
        }
        cout << "------------------- End of Verification I" << endl << endl;
        
        // Verification II
        cout << "------------------- Beginning of Verification II" << endl;        
        for(i=0; i<5; i++){
          // Set writing mode for RAM
	  		tbCE.write(0);
			tbWE.write(0);
          // Set range distribution
		scv_bag<data_range> data_list2;
			data_list2.add(data_range(80,99),5);
			data_list2.add(data_range(100,120),95);
	  // Generate values of tInData  using "cPkt"
			cPkt.sAddr->set_mode(addr_list);
			cPkt.sInData->set_mode(data_list2);
			cPkt.next();
			tAddr.write(cPkt.sAddr->read());
			tInData.write(cPkt.sInData->read());
			sc_start(1);
          // Set reading mode for RAM
	  		tbCE.write(0);
			tbWE.write(1);
			sc_start(1);
          // Data read from tOutData, save to cPkt.sOutData
		  	*cPkt.sOutData = tOutData.read();
          // Print statistics
	  		cPkt.print();
	  		sc_start(1);
        }
        cout << "------------------- End of Verification II" << endl;
        
	// Close trace file
	sc_close_vcd_trace_file(wf);
	
	return 0;	// Terminate simulation
}
