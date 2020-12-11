

   #include "REG5131.h"

 /********************************************************************/
 /***********                      LCD                     ***********/
 /********************************************************************/

   #define LCD_EN P2_0
   #define LCD_RS P2_1
   #define pressed P3_3
   #define DATA_BUS P0       //LCD DATA BUS

 /********************************************************************/
 /***********                    SENSORS                   ***********/
 /********************************************************************/

   #define F_SENSOR P1_6
   #define L_SENSOR P1_5 
   #define R_SENSOR P1_7

 /********************************************************************/
 /***********                     MOTORS                   ***********/
 /********************************************************************/

   #define L_MOTOR1 P2_2
   #define L_MOTOR2 P2_3
   #define R_MOTOR1 P2_4
   #define R_MOTOR2 P2_5

 /********************************************************************/
 /***********                 Globals  + Flash vars        ***********/
 /********************************************************************/

    //	                                     0H  1   2   3   4H  5   6   7   8   9   A   B   
   unsigned char code index_key_array [] = {'1','4','7','*','2','5','8','0','3','6','9','#'};

   unsigned char code msg_array [12][24] ={"      PROJECT BY:       ",  //0
						                   "        VLADI KRUK      ",  //1
                                           "Enter a Code :          ",  //2
                                           "Error Try Again         ",  //3
                                           "Project Open            ",  //4
                                           "Project Are Close       ",  //5
									       "Press: 1-Auto Drive     ",  //6
									       "     Auto Driveing      ",  //7
									       "       2-Self Drive     ",  //8
                                           "        WELCOME         ",  //9
                                           "      CAR PROJECT       ",  //10
                                           "     Self Driveing      "}; //11

   unsigned char key_pressed , msg_no ;

   int step=0 , CODE_flag=0, Start_flag=0;

 /********************************************************************/
 /***********                  Functions                   ***********/
 /********************************************************************/

         /* --Set_LCD-- */
   void init_LCD (void);
   void lcd_cmd_wr(unsigned char value);
   void DATA_BUS_wr(unsigned char value);
   void lcd_msg_wr (int run);

         /* --Print_LCD-- */
   void Print_first_line (int line, int time, int run);
   void Print_second_line (int line, int time, int run);
   void clear_LCD (void);
   void Set_line2 (void);

         /* --Delay-- */
   void delay_ms( unsigned int delay);
   void delay (unsigned int x);

         /* --MOTOR-- */
   void drive (void);
   void stop (void);
   void left (void);
   void right (void);
   void reverse (void);

 /*****************************************************************************************************************/
 /**********************************                                             **********************************/
 /**********************************                                             **********************************/
 /**********************************             MAIN Functions                  **********************************/
 /**********************************                                             **********************************/
 /**********************************                                             **********************************/
 /*****************************************************************************************************************/



   void main (void)

   {
    int i=0, x=0, z=1, sum=0;
    char y;

    /* --  Enable interrupt 1 (Keypad_Interrupt) -- */

    EX1=1;              //Enable external interrupt1.
	IT1=1;              //set edge interrupt 1
	EA = 1;            	// enable interrupts

	/* --  init LCD after power up  -- */

    init_LCD ();

    /* -- PROJECT START POINT --*/

	clear_LCD ();
	Print_first_line (9 , 1 , 1);           // send "WELCOME" TO LCD
	Set_line2 ();
	Print_second_line ( 10 , 1 ,1);       // send "CAR PROJECT" TO LCD
    delay (50);

	clear_LCD ();
	Print_first_line ( 0 , 1 ,1);          //send "~~~ PROJECT BY: ~~~ " TO LCD
	Set_line2 ();
	Print_second_line ( 1 , 1 ,1);        // send "   VLADI KRUK   " TO LCD
    delay (50);

	clear_LCD ();
	Print_first_line (2 , 1, 0);           // send "Enter a Code :" TO LCD
	Set_line2 ();

	key_pressed = 0xff;                 // flag for invalid key

	
	/* --- Enter CODE --- */

	while (i<3) 
    { 	
		       
  		if(key_pressed != 0xff && x<4)
		{
			
			lcd_cmd_wr(0x06);	         // INCREMENT / NO SHIFT
			delay_ms(30);
			DATA_BUS_wr('*') ;
            delay_ms(500);
                           y=key_pressed;
                            if(y=='2'&&sum==0)sum=1;
                            if(y=='5'&&sum==1)sum=2;
                            if(y=='0'&&sum==2)sum=3;
                            if(y=='6'&&sum==3)sum=4;
                           x++;
		
			key_pressed = 0xff;                  // set key_pressed flag for the next loop
	    
         if(x==4)
		 {
		      
		      switch (i)
			  {
			  case 0: case 1:
			         if(sum==4)
					 { 	
					    clear_LCD ();
					    Print_first_line (4 , 20 ,1);        // send "Project Open" TO LCD
						CODE_flag=1; i=3;  
					 }
					 else
					 { 	
					    clear_LCD ();
					    Print_first_line (3 , 20 ,1);        // send "Error Try Again" TO LCD
					 }
                    break;

			  case 2:                               
			         if(sum==4)
					 { 	
					    clear_LCD ();
					    Print_first_line (4 , 20, 1);        // send "Project Open" TO LCD
						CODE_flag=1; i=3;
					 }
					 else
					 { 
	  			        clear_LCD ();
                    	Print_first_line (5 , 1000 ,1);     // send "Project Are Close" TO LCD
					 }
                     break;   

              default :
                       break;
			  }  //switch

          if(i<2)
          {
		    clear_LCD ();
            Print_first_line (2 , 1 ,0);     // send "Enter A Code :" TO LCD
	        Set_line2 ();
          }
		  i++;
          x=0;

		 }                               //if(x==4)
		}                                //if(key_pressed != 0xff && x<4)
	
	}                                    // while (i<3)



      /* --- START MENU --- */

      if(CODE_flag==1)
        {

           clear_LCD ();
           Print_first_line ( 6 , 1 ,1);            // send "Press: 1-Auto Drive" TO LCD
           Set_line2 ();
		   Print_second_line ( 8 , 1 ,1 );               // send "2-Self Drive" TO LCD

           while(z==1)
            {
               if(key_pressed != 0xff )
	               {
                      delay (20);
	                  y=key_pressed;

	                  if(y=='1')
					  { 
					  z=0;
					  Start_flag=1;
                      clear_LCD ();
                      Print_first_line ( 7 , 1, 0);      // send "Auto Driveing" TO LCD
					  }

					  if(y=='2')
					  { 
					  z=0;
					  Start_flag=2;
                      clear_LCD ();
                      Print_first_line ( 11 , 1, 0);      // send "Self Driveing" TO LCD
					  }


	                  key_pressed = 0xff;

	               }                                  //if(key_pressed != 0xff )
           }                                          //while(z==1)
       }                                              //if(CODE_flag==1)

    
        /* --- AUTO DRIVE --- */
    
       while(Start_flag==1)
          {

             if ( F_SENSOR==0 )               
                 step=0;
             else   if ( (L_SENSOR==0) && (R_SENSOR==1) )  
                       step=1;
                   else  if( (R_SENSOR==0) )        
                            step=2;
                          else
                            step=3;

           switch(step)
                {
                  case 0:    drive ();
                         break;
                  case 1:    stop ();
                             delay (20);
                             left ();
                             delay (20);
                         break;
                  case 2:    stop ();
                             delay (20);
                             right ();
                             delay (20);
                         break;
                  case 3:    stop ();
                             delay (20);
						     while ( (L_SENSOR==1) && (R_SENSOR==1) )
                                 reverse ();
                             if( (R_SENSOR==0) )  
                                {      
                                  stop ();
                                  delay (20);
                                  right ();
                                  delay (20);
                                }
                             else
                                {      
                                  stop ();
                                  delay (20);
                                  left ();
                                  delay (20);
                                }
                         break;
                  default: 
                         break;
                 }                            //switch
          }                                   //while drive

       stop ();


	        /* ---- Self Drive ---- */

	     while(Start_flag==2)
		 {
		    if(key_pressed != 0xff )
		    {
		       DATA_BUS_wr(key_pressed);
	           y=key_pressed;

            switch(y)
                {
                  case '2':  drive ();
				             delay (20);
							 stop ();
                         break;
                  case '4':  left ();
                             delay (18);
							 stop ();
                         break;
                  case '6':  right ();
                             delay (18);
							 stop ();
                         break;
                  case '8':  reverse ();
							 delay (20);
							 stop ();
                         break;
                  default:
                         break;
                 }                            //switch

             key_pressed = 0xff;
             y=key_pressed;

             clear_LCD ();
             Print_first_line ( 11 , 1, 0);       // send "Self Driveing" TO LCD
             Set_line2 ();

		   }                                  //if(key_pressed != 0xff )
		 }                                    //while(Start_flag==2)
   }                                          // main


 /************************************************************************************************/
 /************************************************************************************************/
 /*----------------------------------------------------------------------------------------------*/
 /*                                     Delay-functions                                          */
 /*----------------------------------------------------------------------------------------------*/
 /************************************************************************************************/
 /************************************************************************************************/

   void delay (unsigned int x)
   {
    unsigned int y;
    for(;x>0;x--)
    {
     for(y=0;y<30000;y++);
    }
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void delay_ms( unsigned int delay)
   {
   	  unsigned int tmp;
	  for ( ; delay > 0 ; delay--) 
       { 
		   for (tmp = 100 ; tmp > 0 ; tmp --) ;
	   }
   }

 /************************************************************************************************/
 /************************************************************************************************/
 /*----------------------------------------------------------------------------------------------*/
 /*                                     MOTOR-functions                                          */
 /*----------------------------------------------------------------------------------------------*/
 /************************************************************************************************/
 /************************************************************************************************/

   void drive ()
   {
    L_MOTOR1=1;
    L_MOTOR2=0;
    R_MOTOR1=1;
    R_MOTOR2=0;
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void stop ()
   {
    L_MOTOR1=0;
    L_MOTOR2=0;
    R_MOTOR1=0;
    R_MOTOR2=0;
   }

 /********************************************/
 /********************************************/
 /********************************************/

    void left ()
    {
    L_MOTOR1=1;
    L_MOTOR2=0;
    R_MOTOR1=0;
    R_MOTOR2=1;
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void right ()
   {
    L_MOTOR1=0;
    L_MOTOR2=1;
    R_MOTOR1=1;
    R_MOTOR2=0;
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void reverse ()
   {
    L_MOTOR1=0;
    L_MOTOR2=1;
    R_MOTOR1=0;
    R_MOTOR2=1;
   }

 /************************************************************************************************/
 /************************************************************************************************/
 /*----------------------------------------------------------------------------------------------*/
 /*                                     LCD-functions                                            */
 /*----------------------------------------------------------------------------------------------*/
 /************************************************************************************************/
 /************************************************************************************************/

   void init_LCD (void)   
   {
	   delay_ms(100);
	   lcd_cmd_wr(0x30);
	   delay_ms(30);
	   lcd_cmd_wr(0x30);
	   delay_ms(30);
	   lcd_cmd_wr(0x30);
	   delay_ms(30);
       lcd_cmd_wr(0x3C); 
	   delay_ms(30);
	   lcd_cmd_wr(0x0E);  
	   delay_ms(30);
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void lcd_cmd_wr(unsigned char value)
   {
	   LCD_EN  = 0;
	   LCD_RS = 0 ;
	   delay_ms(1);
	   LCD_EN  = 1;
	   delay_ms(1);
	   DATA_BUS = value ;	// data output
	   delay_ms(1);
	   LCD_EN  = 0;		// write transient
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void DATA_BUS_wr(unsigned char value) 
   {
   	   LCD_EN  = 0;
	   LCD_RS = 1 ;		// enable data write 
	   delay_ms(1);
       LCD_EN  = 1;
	   delay_ms(1);
	   DATA_BUS = value ;	// data output
	   delay_ms(1);
	   LCD_EN  = 0;		// write transient
	   LCD_RS = 0 ;		// return to register write 
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void lcd_msg_wr (int run) 
   { 
       unsigned char cntr;
	   for (cntr = 0; cntr < 24 ; cntr ++)
       {
		   DATA_BUS_wr (msg_array[msg_no][cntr]) ;
           if (run)
           delay_ms(150);
	   }
   }	

 /************************************************************************************************/
 /************************************************************************************************/
 /*----------------------------------------------------------------------------------------------*/
 /*                                  Print_LCD-functions                                         */
 /*----------------------------------------------------------------------------------------------*/
 /************************************************************************************************/
 /************************************************************************************************/

   void Print_first_line (int line, int time, int run)
   {
      	   msg_no = line ; 
           lcd_msg_wr(run);  // send ""word line" TO LCD
       	   delay (time);
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void clear_LCD (void)
   { 
           lcd_cmd_wr(0x01);	// clear display
           delay_ms(30);     
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void Set_line2 (void)
   {
	   lcd_cmd_wr(0xC0);		// set lcd address to line 2 start
       delay_ms(1);
   }

 /********************************************/
 /********************************************/
 /********************************************/

   void Print_second_line (int line, int time, int run)
   {

	   msg_no = line ; 
	   lcd_msg_wr(run);  // send "word line"  TO LCD
       delay (time);
   }

 /************************************************************************************************/
 /************************************************************************************************/
 /*----------------------------------------------------------------------------------------------*/
 /*                                     INT_1-functions                                          */
 /*----------------------------------------------------------------------------------------------*/
 /************************************************************************************************/
 /************************************************************************************************/

   void interrupt1 (void) interrupt 2 using 2 //Keypad interrupt (keypad 3*4)
   {
	   unsigned char key_from_keypad_enc;

	   key_from_keypad_enc=(0x0f & P1);//GET THE VALUE OF PRESSED KEY FROM KEYPAD ENCODER
	   key_pressed = index_key_array[key_from_keypad_enc];

       if (Start_flag==1)
	   {
	   Start_flag=0;
       CODE_flag=0;
	   }

	   if (Start_flag==2)
	   {
          if( key_pressed == '5' )
           {
	         Start_flag=0;
             CODE_flag=0;
	   }}
    }

 /********************************************/
 /********************************************/
 /********************************************/
