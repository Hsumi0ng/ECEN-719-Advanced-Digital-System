//===========================================
// Function : Asynchronous SRAM 
//===========================================
#include "systemc.h"

#define DATA_WIDTH        8 
#define ADDR_WIDTH        18 
#define RAM_DEPTH         1 << ADDR_WIDTH

SC_MODULE (RAM) {
  // ----- Declare Input/Output ports -----
  sc_in <sc_uint<ADDR_WIDTH> > Addr;
  sc_in <bool> bWE;
  sc_in <bool> bCE;
  sc_in <sc_uint<DATA_WIDTH> > InData;
  sc_out <sc_uint<DATA_WIDTH> > OutData;

  // ----- Internal variables -----
   sc_uint <DATA_WIDTH>  SRAM [RAM_DEPTH];

  // ----- Code Starts Here -----     
  // Memory Write Block 
  // Write Operation : When we_b = 0, ce_b = 0
  void writemode(){
    if( bWE.read()==0 && bCE.read() ==0)
      SRAM [Addr.read()] = InData.read();
  }

  
  // Memory Read Block 
  // Read Operation : When we_b = 1, ce_b = 0
  void readmode(){
    if (bWE.read()==1 && bCE.read()==0)
    OutData.write(SRAM[Addr.read()]);
}

  // ----- Constructor for the SC_MODULE -----
  // sensitivity list
  SC_CTOR(RAM) 

      SC_METHOD(writemode);
   sensitive << Addr << bWE << bCE << InData ;
    SC_METHOD(readmode);
    sensitive << Addr << bWE << bCE << OutData ;
	// ...
  }
};

