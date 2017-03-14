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
    		switch(flag_src) //��ʼ��ַ�ĸ������
    		{
    			case 0: //���ֶ�
                                
    				temp=(*(int*)(src_addr));
    				src_addr+=4;
    				num-=4;
    				break;
    			case 1://�ȶ�һ���ֽڣ��ٰ����ֶ�������ֶ���ƴ�ӳ�һ���ַ���temp
    				temp1=(*(char*)(src_addr));
    				temp2=(*(short*)(src_addr+1));
    				temp3=(*(int*)(src_addr+3));
    				temp=temp1|(temp2<<8)|(temp3<<24);
    				src_addr+=4;
    				num-=4;
    				break;
    			case 2: //�Ȱ����ֶ����ٰ��ֶ���ƴ�ӳ�һ���ַ���temp
    				temp1=(*(short*)(src_addr));
    				temp2=(*(int*)(src_addr+2));
    				temp=temp1|(temp2<<16);
    				src_addr+=4;
    				num-=4;
    				break;
    			case 3: //�ȶ�һ���ֽڣ��ٰ��ֶ���ƴ�ӳ�һ���ַ���temp
    				temp1=(*(char*)(src_addr));
    				temp2=(*(int*)(src_addr+1));
    				temp=temp1|(temp2<<8);
    				src_addr+=4;
    				num-=4;
    				break;
    		}
    }
    
    switch(flag_dst) //����Ŀ�ĵ�ַ�ĸ������
    {
    			case 0://����д
    				(*(int*)(dst_addr))=temp;
    				dst_addr+=4; 			
    				break;
    			case 1: //��дһ���ֽڣ��ٰ�����д���˺��ÿ�οɰ���д
    			    (*(char*)(dst_addr))=(char)temp;
    			    (*(short*)(dst_addr+1))=(short)(temp>>8);
    				src_addr-=1;
    				num+=1;
    				dst_addr+=3;
    				break;
    			case 2: //��дһ�����֣��˺��ÿ�οɰ���д
			        (*(short*)(dst_addr))=(short)temp;
			        src_addr-=2;
			        num+=2;
    				dst_addr+=2;
    				break;
    			case 3: //��дһ���ֽڣ��˺��ÿ�οɰ���д
    			    (*(char*)(dst_addr))=(char)temp;
    				src_addr-=3;
    				num+=3;
    				dst_addr+=1;	
    				break;
    }
	for(i=0;num>3;i++) //�����Ŀ�ĵ�ַ����������󣬿���ֱ�Ӱ���д
	{
		    		(*(int*)(dst_addr))=temp;
    				dst_addr+=4; 
	}
  
   	if(num<4)//ʣ����޷�����һ���ֵ��ֽڵĶ�д��
    {
    	    for(i=0;i<num;i++)
    	    {
    	        (*(char*)(dst_addr))= (*(char*)(src_addr));
    	        src_addr++;
    	        dst_addr++;	        
    	    }
    }

 
}
