extern int unsigned mixop(int p1,int p2, int p3);
 void Test03();
 void Test03()
 {
 	  int result;
          int x,y,z;
          uart_printf("ʵ���� ����C���Ե��໥����/n");
          uart_printf("��ʵ��ʵ���˼�����ʽx+y*z\n");
          uart_printf("����������x��\n");
          x=uart_getch();
          uart_printf("����������y��\n");
          y=uart_getch();
          uart_printf("����������z��\n");
          z=uart_getch();
          result = mixop(6,40,50);
          uart_printf("%d+%d*%d�Ľ��Ϊ��%d\n",x,y,z,result);
	  
 }
