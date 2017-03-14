extern int unsigned mixop(int p1,int p2, int p3);
 void Test03();
 void Test03()
 {
 	  int result;
          int x,y,z;
          uart_printf("实验三 汇编和C语言的相互调用/n");
          uart_printf("该实验实现了计算表达式x+y*z\n");
          uart_printf("请输入整数x：\n");
          x=uart_getch();
          uart_printf("请输入整数y：\n");
          y=uart_getch();
          uart_printf("请输入整数z：\n");
          z=uart_getch();
          result = mixop(6,40,50);
          uart_printf("%d+%d*%d的结果为：%d\n",x,y,z,result);
	  
 }
