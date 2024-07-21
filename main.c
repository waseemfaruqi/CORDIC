/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xil_io.h"
#include "../../design_1_wrapper_hw_platform_0/drivers/complex_mul_v1_0/src/complex_mul.h"

#define BASEADDR XPAR_COMPLEX_MUL_0_S00_AXI_BASEADDR
#define num_test 2

// test vectors and expected results
static uint32_t yx[num_test] = {0xb500b5, 0xff4bff4b};
static uint32_t kA[num_test] = {0x8000c9, 0xb5ff37};

/*	k_alpha	=> reg0
 * 	YX_in	=> reg1
 * 	reset	=> reg2(0)
 * 	i_valid	=> reg2(1)
 * 	o_valid	=> reg3(0)
 * 	YX_out	=> reg4
 * */
int main()
{
    int i;
    uint32_t out;
	init_platform();

    print("Hello World\n\r");

    COMPLEX_MUL_mWriteReg(BASEADDR, 8, 0); //valid=0 resetn=0

    for( i = 0; i < num_test; i++){
    	COMPLEX_MUL_mWriteReg(BASEADDR, 8, 1); //valid=0 resetn=1
    	COMPLEX_MUL_mWriteReg(BASEADDR, 4, yx[i]); //apply XY
    	COMPLEX_MUL_mWriteReg(BASEADDR, 0, kA[i]); //apply k - Alpha
    	COMPLEX_MUL_mWriteReg(BASEADDR, 8, 3); //valid=1 resetn=1
    	while(!COMPLEX_MUL_mReadReg(BASEADDR, 12));
    	out = COMPLEX_MUL_mReadReg(BASEADDR, 16);
    	xil_printf("carte_xy = %.8x \r\n", out);
    }
    cleanup_platform();
    return 0;
}
