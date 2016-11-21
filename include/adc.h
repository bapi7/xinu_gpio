struct  adc_csreg
{
        volatile uint32 data1;
};

/*struct  uart_csreg
{
        volatile uint32 buffer; // receive buffer (when read)           
                                //   OR transmit hold (when written)    
        volatile uint32 ier;    // interrupt enable                     
        volatile uint32 iir;    // interrupt identification (when read) 
                                //   OR FIFO control (when written)     
        volatile uint32 lcr;    // line control register                
        volatile uint32 mcr;    // modem control register               
        volatile uint32 lsr;    // line status register                 
        volatile uint32 msr;    // modem status register                
        volatile uint32 spr;    // scratch register                     
        volatile uint32 mdr1;
        volatile uint32 res[12];// unused UART registers                
        volatile uint32 sysc;   // system configuration register        
        volatile uint32 syss;   // system status register               
        volatile uint32 wer;
        volatile uint32 res4;
        volatile uint32 rxfifo_lvl;
        volatile uint32 txfifo_lvl;
        volatile uint32 ier2;
        volatile uint32 isr2;
        volatile uint32 freq_sel;
        volatile uint32 res5[2];
        volatile uint32 mdr3;
        volatile uint32 tx_dma_thresh;
};*/

/* Alternative names for control and status registers */
/*
#define rbr     buffer          // receive buffer (when read)           
#define thr     buffer          // transmit hold (when written)         
#define fcr     iir             // FIFO control (when written)          
#define dll     buffer          // divisor latch (low byte)             
#define dlm     ier             // divisor latch (high byte)            
*/

/* Definintion of individual bits in control and status registers       */
#define STEP_ENABLE	0x44E0D054
#define STEP_CONFIG	0x44E0D06C
#define STEP_DELAY	0x44E0D070
