#include "rw_ram.h"

void c_move(int src_addr, int num, int dst_addr)
{
    int i,j,nStep;
    int flag_src,flag_dst,tag;
    int temp,temp1,temp2,temp3;
    short temp_short;
    char temp_char;
    int tag_read,tag_write;
    tag_read=src_addr+num;
    tag_write=dst_addr+num;
    

    flag_src = src_addr % 4;
    flag_dst = dst_addr % 4;
        
    while(src_addr!=tag_read&&num>3)
    {
    		switch(flag_src) //起始地址的各种情况
    		{
    			case 0: //按字读
                                
    				temp=(*(int*)(src_addr));
    				src_addr+=4;
    				num-=4;
    				break;
    			case 1://先读一个字节，再按半字读，最后按字读，拼接出一个字放在temp
    				temp1=(*(char*)(src_addr));
    				temp2=(*(short*)(src_addr+1));
    				temp3=(*(int*)(src_addr+3));
    				temp=temp1|(temp2<<8)|(temp3<<24);
    				src_addr+=4;
    				num-=4;
    				break;
    			case 2: //先按半字读，再按字读，拼接出一个字放在temp
    				temp1=(*(short*)(src_addr));
    				temp2=(*(int*)(src_addr+2));
    				temp=temp1|(temp2<<16);
    				src_addr+=4;
    				num-=4;
    				break;
    			case 3: //先读一个字节，再按字读，拼接出一个字放在temp
    				temp1=(*(char*)(src_addr));
    				temp2=(*(int*)(src_addr+1));
    				temp=temp1|(temp2<<8);
    				src_addr+=4;
    				num-=4;
    				break;
    		}
    }
    
    switch(flag_dst) //考虑目的地址的各种情况
    {
    			case 0://按字写
    				(*(int*)(dst_addr))=temp;
    				dst_addr+=4; 			
    				break;
    			case 1: //先写一个字节，再按半字写，此后的每次可按字写
    			    (*(char*)(dst_addr))=(char)temp;
    			    (*(short*)(dst_addr+1))=(short)(temp>>8);
    				src_addr-=1;
    				num+=1;
    				dst_addr+=3;
    				break;
    			case 2: //先写一个半字，此后的每次可按字写
			        (*(short*)(dst_addr))=(short)temp;
			        src_addr-=2;
			        num+=2;
    				dst_addr+=2;
    				break;
    			case 3: //先写一个字节，此后的每次可按字写
    			    (*(char*)(dst_addr))=(char)temp;
    				src_addr-=3;
    				num+=3;
    				dst_addr+=1;	
    				break;
    }
	for(i=0;num>3;i++) //填充完目的地址不对齐情况后，可以直接按字写
	{
		    		(*(int*)(dst_addr))=temp;
    				dst_addr+=4; 
	}
  
   	if(num<4)//剩余的无法构成一个字的字节的读写！
    {
    	    for(i=0;i<num;i++)
    	    {
    	        (*(char*)(dst_addr))= (*(char*)(src_addr));
    	        src_addr++;
    	        dst_addr++;	        
    	    }
    }

 
}
