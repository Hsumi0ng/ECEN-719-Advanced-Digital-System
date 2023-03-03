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
		// ...
		// ...
		// ...
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
	RAM RAM_01("SIMULATION_RAM");
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

        // Verification I
        cout << "------------------- Beginning of Verification I" << endl;
        for(i=0; i<5; i++){
          // Set writing mode for RAM
	  // ...
 
          // Generate values for tInData & tAddr using "cPkt"
	  // ...
	           
          // Set reading mode for RAM
	  // ...

          // Data read from tOutData, save to cPkt.sOutData
	  // ...

          // Print statistics
	  cPkt.print();
	  sc_start(1);
        }
        cout << "------------------- End of Verification I" << endl << endl;
        
        // Verification II
        cout << "------------------- Beginning of Verification II" << endl;        
        for(i=0; i<5; i++){
          // Set writing mode for RAM
	  // ...
         
          // Set range distribution
	  // ...
          
	  // Generate values of tInData  using "cPkt"
	  // ...
          
          // Set reading mode for RAM
	  // ...

          // Data read from tOutData, save to cPkt.sOutData
	  // ...

          // Print statistics
	  // ...
        }
        cout << "------------------- End of Verification II" << endl;
        
	// Close trace file
	sc_close_vcd_trace_file(wf);
	
	return 0;	// Terminate simulation
}
