//===========================================
// Function : UART Transmitter
//===========================================
#include "UART_XMTR.h"

  // ----- Code Starts Here ----- 

  void UART_XMTR::Send_bit() {
                       switch(IntState)  //STATE_MACHINE
		{
			case STATE_IDLE:
                          if( Load_XMT_datareg.read() == 1){
                            XMT_datareg  = Data_Bus.read();                       
                            NextIntState = STATE_IDLE;
                          }
                          else if ( Byte_ready.read() == 1){                         
                            XMT_shftreg  = (XMT_datareg.range(WORD_SIZE-1,0),'1');
                            NextIntState = STATE_WAITING;                           
                          }
                          else{
                            NextIntState = STATE_IDLE;                              
                          }
                          break;
                                
			case STATE_WAITING:
			  if( T_byte.read() == 1){                                    
                            XMT_shftreg  = XMT_shftreg-1;
                            NextIntState = STATE_SENDING;                          
                          }
                          else{
                            NextIntState = STATE_WAITING;
                          }
                          break;
			
			case STATE_SENDING:
                          if( bit_count <= WORD_SIZE+1){             
                            XMT_shftreg = ('1',XMT_shftreg.range(8,1));             
                            bit_count  = bit_count + 1;                              
                            NextIntState = STATE_SENDING;
                          }
                          else{
                            bit_count = 0;                                        
                            NextIntState = STATE_IDLE;
                          }
                          break;
	    
	    default: {
				NextIntState = STATE_IDLE;
	    }

		}

                       Serial_out.write(XMT_shftreg[0]);             
  
  }

  void UART_XMTR::Initialize() {
		if(!rst_b.read()) {
			IntState = STATE_IDLE;
			
			XMT_shftreg = 0x1ff;
			bit_count = 0;
		}
		else {
			IntState = NextIntState;
		}
  }
