

//プロトタイプ宣言

//start画面の表示
void start(void);
//引数（演算子以外）をディスプレイに表示させる関数
void display_value(int fig);
/*
演算子をディスプレイに表示する関数
display_operator(0)は+の表示
display_operator(1)は-の表示
display_operator(2)は*の表示
display_operator(3)/の表示
display_operator(4)は=の表示
*/
void display_operator(int operator);

//必要なライブラリをinclude
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include<math.h>

#define SIZE 100
#define NULL 0




int main()
{

        int i,j,k;
        int count=0;

        int answer;
        int correct_answer;

        int key_value;
        int game_select_value=0;
        int random_operator,random_value[2];
        int value_count = 0,operator_count = 0;
        int operator[SIZE],value_array[SIZE];
        int value;
        bool flag14 = false;
        bool flag13 = true;
        bool flag11 = true;
        bool flag7 = false;
        bool flag_start = false;
        bool flag_reset = false;
        bool flag_calcurate = false;
        bool flag_random = false;
        int seed=0;







        //数字の0を7segに表示
        *(volatile unsigned char *)HEX0_BASE = 64;
        *(volatile unsigned char *)HEX1_BASE = 64;
        *(volatile unsigned char *)HEX2_BASE = 64;
        *(volatile unsigned char *)HEX3_BASE = 64;
        *(volatile unsigned char *)HEX4_BASE = 64;
        *(volatile unsigned char *)HEX5_BASE = 64;
        *(volatile unsigned char *)HEX6_BASE = 64;
        *(volatile unsigned char *)HEX7_BASE = 64;

    	*(volatile unsigned char *)KEY_BASE = 0;



        //printf("Hello\n");
    	while(1){
    		if(flag_start == false){
    				flag_start = true;
    				flag_random = false;
    		        seed++;
    				start();

    		}

            value = *(volatile unsigned char *)SWITCHES_BASE;


                  *(volatile unsigned char *)HEX1_BASE = 64;
                  *(volatile unsigned char *)HEX2_BASE = 64;
                  *(volatile unsigned char *)HEX3_BASE = 64;
                  switch(value){
                  						case 0:
                  								*(volatile unsigned char *)HEX0_BASE = 64;
                  								break;
                  						case 1:
                  								*(volatile unsigned char *)HEX0_BASE = 121;
                  								break;
                  						case 2:
                  								*(volatile unsigned char *)HEX0_BASE = 36;
                  								break;
                  						case 3:
                  								*(volatile unsigned char *)HEX0_BASE = 48;
                  								break;
                  						case 4:
                  								*(volatile unsigned char *)HEX0_BASE = 25;
                  								break;
                  						case 5:
                  								*(volatile unsigned char *)HEX0_BASE = 18;
                  								break;
                  						case 6:
                  								*(volatile unsigned char *)HEX0_BASE = 2;
                  								break;
                  						case 7:
                  								*(volatile unsigned char *)HEX0_BASE = 88;
                  								break;
                  						case 8:
                  								*(volatile unsigned char *)HEX0_BASE = 0;
                  								break;
                  						case 9:
                  								*(volatile unsigned char *)HEX0_BASE = 24;
                  								break;
                  						}
    	          /*
    	          1か2をスイッチで選択し、KEY0を押すことでゲームを決定する
    	          1:calcurate game
    	          2:calcurator
    	          */
    	          if(*(volatile unsigned char *)KEY_BASE==14 || *(volatile unsigned char *)KEY_BASE==13 || *(volatile unsigned char *)KEY_BASE==11 || *(volatile unsigned char *)KEY_BASE==7){
    	              game_select_value = *(volatile unsigned char *)SWITCHES_BASE;
    	              if(game_select_value==1){
    	          		if(flag_reset == false){
    	          				flag_reset = true;
    	          				reset();
    	          		}
    	          		//printf("%d\n",(unsigned int)time(NULL));

    	                while (1) {
    	                  //calcurate game code
    	    	              answer = *(volatile unsigned char *)SWITCHES_BASE;

    	    	                //複数桁の数字を7seg表示
    	    	                if(answer>=10){
    	    	                  int a[4];
    	    	                  int multi_value;
    	    	                  multi_value = answer;
    	    	                  a[0]=(multi_value%10);multi_value/=10;//1桁目
    	    	                  a[1]=(multi_value%10);multi_value/=10;//2桁目
    	    	                  a[2]=(multi_value%10);multi_value/=10;//3桁目
    	    	                  a[3]=(multi_value%10);multi_value/=10;//4桁目

    	    	                  switch(a[0]){
    	    	                  	  case 0:
    	    	                  		  *(volatile unsigned char *)HEX0_BASE = 64;
    	    	                  		  break;
    	    	                      case 1:
    	    	                    	  *(volatile unsigned char *)HEX0_BASE = 121;
    	    	                          break;
    	    	                      case 2:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 36;
    	    	                          break;
    	    	                      case 3:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 48;
    	    	                          break;
    	    	                      case 4:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 25;
    	    	                          break;
    	    	                      case 5:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 18;
    	    	                          break;
    	    	                      case 6:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 2;
    	    	                          break;
    	    	                      case 7:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 88;
    	    	                          break;
    	    	                      case 8:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 0;
    	    	                          break;
    	    	                      case 9:
    	    	                          *(volatile unsigned char *)HEX0_BASE = 24;
    	    	                          break;
    	    	                          }
    	    	                  switch(a[1]){
    	    	                        case 0:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 64;
    	    	                            break;
    	    	                        case 1:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 121;
    	    	                            break;
    	    	                        case 2:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 36;
    	    	                            break;
    	    	                        case 3:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 48;
    	    	                            break;
    	    	                        case 4:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 25;
    	    	                            break;
    	    	                        case 5:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 18;
    	    	                            break;
    	    	                        case 6:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 2;
    	    	                            break;
    	    	                        case 7:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 88;
    	    	                            break;
    	    	                        case 8:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 0;
    	    	                            break;
    	    	                        case 9:
    	    	                            *(volatile unsigned char *)HEX1_BASE = 24;
    	    	                            break;
    	    	                            }
    	    	                  switch(a[2]){
    	    	                        case 0:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 64;
    	    	                            break;
    	    	                        case 1:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 121;
    	    	                            break;
    	    	                        case 2:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 36;
    	    	                            break;
    	    	                        case 3:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 48;
    	    	                            break;
    	    	                        case 4:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 25;
    	    	                            break;
    	    	                        case 5:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 18;
    	    	                            break;
    	    	                        case 6:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 2;
    	    	                            break;
    	    	                        case 7:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 88;
    	    	                            break;
    	    	                        case 8:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 0;
    	    	                            break;
    	    	                        case 9:
    	    	                            *(volatile unsigned char *)HEX2_BASE = 24;
    	    	                            break;
    	    	                            }
    	    	                  switch(a[3]){
    	    	                        case 0:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 64;
    	    	                            break;
    	    	                        case 1:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 121;
    	    	                            break;
    	    	                        case 2:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 36;
    	    	                            break;
    	    	                        case 3:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 48;
    	    	                            break;
    	    	                        case 4:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 25;
    	    	                            break;
    	    	                        case 5:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 18;
    	    	                            break;
    	    	                        case 6:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 2;
    	    	                            break;
    	    	                        case 7:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 88;
    	    	                            break;
    	    	                        case 8:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 0;
    	    	                            break;
    	    	                        case 9:
    	    	                            *(volatile unsigned char *)HEX3_BASE = 24;
    	    	                            break;
    	    	                        }
    	    	                }
    	    	                else{
    	    	                  *(volatile unsigned char *)HEX1_BASE = 64;
    	    	                  *(volatile unsigned char *)HEX2_BASE = 64;
    	    	                  *(volatile unsigned char *)HEX3_BASE = 64;
    	    	                  switch(answer){
    	    	                  						case 0:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 64;
    	    	                  								break;
    	    	                  						case 1:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 121;
    	    	                  								break;
    	    	                  						case 2:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 36;
    	    	                  								break;
    	    	                  						case 3:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 48;
    	    	                  								break;
    	    	                  						case 4:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 25;
    	    	                  								break;
    	    	                  						case 5:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 18;
    	    	                  								break;
    	    	                  						case 6:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 2;
    	    	                  								break;
    	    	                  						case 7:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 88;
    	    	                  								break;
    	    	                  						case 8:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 0;
    	    	                  								break;
    	    	                  						case 9:
    	    	                  								*(volatile unsigned char *)HEX0_BASE = 24;
    	    	                  								break;
    	    	                  						}
    	    	                }


          	          		if(flag_calcurate == false){
          	          				flag_calcurate = true;


          	          				for(i=0;i<10;i++){

          	          				//rand()%(B-A+1)+A; AからBまでのランダム整数
          	          				if(flag_random == false){
          	          					flag_random = true;
          	          					srand(seed);
          	          				}

    	                	random_operator = rand()%3;//0から3までの整数(演算子の生成)
    	                	random_value[0] = rand()%10+1;//から10までの整数
    	                	random_value[1] = rand()%100+1;//1から100までの整数



      	          				if(random_operator == 0){
      	          					correct_answer = random_value[1] + random_value[0];
      	          				}
      	          				else if(random_operator == 1){
      	          					correct_answer = random_value[1] - random_value[0];
      	          				}
      	          				else if(random_operator == 2){
      	          					correct_answer = random_value[1] * random_value[0];
      	          				}
      	          				else if(random_operator == 3){
      	          					correct_answer = random_value[1] / random_value[0];
      	          				}
      	          				printf("correct_ans=%d\n",correct_answer);
      	          		//}
      	          				if(correct_answer > 0 && correct_answer < 255){break;}
      	          				else{
      	          					printf("miss\n");
      	          				}
          	          		}
              	                  //画面表示
                	          				display_value(random_value[1]);
                	          				display_operator(random_operator);
                	          				display_value(random_value[0]);
                	          				display_operator(4);
          	          		}

    	                  //calcurate game のループをbreakし、ゲーム選択画面に戻る
    	                  if(*(volatile unsigned char *)KEY_BASE==0){
    	                	flag_start = false;
    	                	flag_reset = false;
               				flag_calcurate = false;
               		        flag14 = false;
               		        flag13 = true;
               		        flag11 = true;
               		        flag7 = true;

    	                	reset();
    	                    break;
    	                  }

    	                  switch(*(volatile unsigned char *)KEY_BASE){
    	                   //KEY0を押すと入力された数字を表示し、value_arrayに代入
    	                   	case 14:
    	                   		flag13 = false;
    	                   		flag11 = false;
    	                   		flag7 = false;
    	                   		if(flag14 == false){
    	                   			flag14 = true;
    	                   			display_value(answer);
    	                   			if(answer == correct_answer){
    	                   				congratulations();
    	                   				flag_calcurate = false;
    	                   		}else{
    	                   			wrong();
	                   				flag_calcurate = false;
    	                   		}
    	                   			reset();
    	                   		}
    	                           break;


    	                   /*
    	                   KEY1を押すと演算をどれにするかoperatorに格納

    	                   */

    	                   	case 13:
    	                   		flag14 = false;
    	                   		flag11 = false;
    	                   		flag7 = false;

    	                   		if(flag13 == false){
    	                   			flag13 = true;
       	                   			display_value(answer);
        	                   			if(answer == correct_answer){
        	                   				congratulations();
        	                   				flag_calcurate = false;
        	                   		}else{
        	                   			wrong();
    	                   				flag_calcurate = false;
        	                   		}
        	                   			reset();
    	                   		}
    	               			break;



    	                   /*
    	                   KEY2を押すと計算が実行され、結果が表示される
    	                   */
    	                   case 11:
    	               		    flag14 = false;
    	               		    flag13 = false;
    	               		    flag7 = false;

    	               		    if(flag11 == false){
    	               			       flag11 = true;
    	               			       reset();
    	               			       flag_calcurate = false;
    	               		    break;



    	                   //KEY3が押されるとリセット
    	                           case 7:
    	                       		  flag14 = false;
    	                       		  flag13 = false;
    	                       		  flag11 = false;

    	                       		  if(flag7 == false){
    	                       			     flag7 = true;
    	                       			     reset();
    	      	               			     flag_calcurate = false;
    	                       		  }
    	                       		  break;
    	           }

    	                  /*
    	                   bool値確認↓(key3:key2:key1:key0)
    	                   KEY操作のプログラムでは同じKEYが連続で押されることが無いようにしている
    	                   それぞれのKEYにflagと呼ばれるブール値を代入させる変数を作っていて、
    	                   flagがfalseのときだけそのKEYを押すことが出来る。
    	                   動作例）
    	                   KEY0が押された場合、KEY1,2,3のflagをfalseとし,KEY0のflagをtrueとしている。
    	                   この動作をそれぞれのKEYで行っている。
    	                   */
    	                           //key3
    	                           if(flag7 == true){
    	                               *(volatile unsigned char *)HEX7_BASE = 121;
    	                           }else{
    	                               *(volatile unsigned char *)HEX7_BASE = 64;
    	                           }
    	                           //key2
    	                           if(flag11 == true){
    	                               *(volatile unsigned char *)HEX6_BASE = 121;
    	                           }else{
    	                               *(volatile unsigned char *)HEX6_BASE = 64;
    	                           }
    	                           //key1
    	                           if(flag13 == true){
    	                               *(volatile unsigned char *)HEX5_BASE = 121;
    	                           }else{
    	                               *(volatile unsigned char *)HEX5_BASE = 64;
    	                           }
    	                           //key0
    	                           if(flag14 == true){
    	                               *(volatile unsigned char *)HEX4_BASE = 121;
    	                           }else{
    	                               *(volatile unsigned char *)HEX4_BASE = 64;
    	                           }
    	                }
    	                }
    	              }
    	              else if(game_select_value==2){
      	          		if(flag_reset == false){
      	          				flag_reset = true;
      	          				reset();
      	          		}
        while(1){
            //スイッチとLEDを連動させている
                value = *(volatile unsigned char *)SWITCHES_BASE;
                *(volatile unsigned char *)LEDS_BASE = value;

                if(*(volatile unsigned char *)KEY_BASE==0){
                	flag_start = false;
                	flag_reset = false;
       		        flag14 = false;
       		        flag13 = true;
       		        flag11 = true;
       		        flag7 = true;
                	reset();
                	reset();
                  break;
                }



                //複数桁の数字を7seg表示
                if(value>=10){
                  int a[4];
                  int multi_value;
                  multi_value = value;
                  a[0]=(multi_value%10);multi_value/=10;//1桁目
                  a[1]=(multi_value%10);multi_value/=10;//2桁目
                  a[2]=(multi_value%10);multi_value/=10;//3桁目
                  a[3]=(multi_value%10);multi_value/=10;//4桁目

                  switch(a[0]){
                  	  case 0:
                  		  *(volatile unsigned char *)HEX0_BASE = 64;
                  		  break;
                      case 1:
                    	  *(volatile unsigned char *)HEX0_BASE = 121;
                          break;
                      case 2:
                          *(volatile unsigned char *)HEX0_BASE = 36;
                          break;
                      case 3:
                          *(volatile unsigned char *)HEX0_BASE = 48;
                          break;
                      case 4:
                          *(volatile unsigned char *)HEX0_BASE = 25;
                          break;
                      case 5:
                          *(volatile unsigned char *)HEX0_BASE = 18;
                          break;
                      case 6:
                          *(volatile unsigned char *)HEX0_BASE = 2;
                          break;
                      case 7:
                          *(volatile unsigned char *)HEX0_BASE = 88;
                          break;
                      case 8:
                          *(volatile unsigned char *)HEX0_BASE = 0;
                          break;
                      case 9:
                          *(volatile unsigned char *)HEX0_BASE = 24;
                          break;
                          }
                  switch(a[1]){
                        case 0:
                            *(volatile unsigned char *)HEX1_BASE = 64;
                            break;
                        case 1:
                            *(volatile unsigned char *)HEX1_BASE = 121;
                            break;
                        case 2:
                            *(volatile unsigned char *)HEX1_BASE = 36;
                            break;
                        case 3:
                            *(volatile unsigned char *)HEX1_BASE = 48;
                            break;
                        case 4:
                            *(volatile unsigned char *)HEX1_BASE = 25;
                            break;
                        case 5:
                            *(volatile unsigned char *)HEX1_BASE = 18;
                            break;
                        case 6:
                            *(volatile unsigned char *)HEX1_BASE = 2;
                            break;
                        case 7:
                            *(volatile unsigned char *)HEX1_BASE = 88;
                            break;
                        case 8:
                            *(volatile unsigned char *)HEX1_BASE = 0;
                            break;
                        case 9:
                            *(volatile unsigned char *)HEX1_BASE = 24;
                            break;
                            }
                  switch(a[2]){
                        case 0:
                            *(volatile unsigned char *)HEX2_BASE = 64;
                            break;
                        case 1:
                            *(volatile unsigned char *)HEX2_BASE = 121;
                            break;
                        case 2:
                            *(volatile unsigned char *)HEX2_BASE = 36;
                            break;
                        case 3:
                            *(volatile unsigned char *)HEX2_BASE = 48;
                            break;
                        case 4:
                            *(volatile unsigned char *)HEX2_BASE = 25;
                            break;
                        case 5:
                            *(volatile unsigned char *)HEX2_BASE = 18;
                            break;
                        case 6:
                            *(volatile unsigned char *)HEX2_BASE = 2;
                            break;
                        case 7:
                            *(volatile unsigned char *)HEX2_BASE = 88;
                            break;
                        case 8:
                            *(volatile unsigned char *)HEX2_BASE = 0;
                            break;
                        case 9:
                            *(volatile unsigned char *)HEX2_BASE = 24;
                            break;
                            }
                  switch(a[3]){
                        case 0:
                            *(volatile unsigned char *)HEX3_BASE = 64;
                            break;
                        case 1:
                            *(volatile unsigned char *)HEX3_BASE = 121;
                            break;
                        case 2:
                            *(volatile unsigned char *)HEX3_BASE = 36;
                            break;
                        case 3:
                            *(volatile unsigned char *)HEX3_BASE = 48;
                            break;
                        case 4:
                            *(volatile unsigned char *)HEX3_BASE = 25;
                            break;
                        case 5:
                            *(volatile unsigned char *)HEX3_BASE = 18;
                            break;
                        case 6:
                            *(volatile unsigned char *)HEX3_BASE = 2;
                            break;
                        case 7:
                            *(volatile unsigned char *)HEX3_BASE = 88;
                            break;
                        case 8:
                            *(volatile unsigned char *)HEX3_BASE = 0;
                            break;
                        case 9:
                            *(volatile unsigned char *)HEX3_BASE = 24;
                            break;
                        }
                }
                else{
                  *(volatile unsigned char *)HEX1_BASE = 64;
                  *(volatile unsigned char *)HEX2_BASE = 64;
                  *(volatile unsigned char *)HEX3_BASE = 64;
                  switch(value){
                  						case 0:
                  								*(volatile unsigned char *)HEX0_BASE = 64;
                  								break;
                  						case 1:
                  								*(volatile unsigned char *)HEX0_BASE = 121;
                  								break;
                  						case 2:
                  								*(volatile unsigned char *)HEX0_BASE = 36;
                  								break;
                  						case 3:
                  								*(volatile unsigned char *)HEX0_BASE = 48;
                  								break;
                  						case 4:
                  								*(volatile unsigned char *)HEX0_BASE = 25;
                  								break;
                  						case 5:
                  								*(volatile unsigned char *)HEX0_BASE = 18;
                  								break;
                  						case 6:
                  								*(volatile unsigned char *)HEX0_BASE = 2;
                  								break;
                  						case 7:
                  								*(volatile unsigned char *)HEX0_BASE = 88;
                  								break;
                  						case 8:
                  								*(volatile unsigned char *)HEX0_BASE = 0;
                  								break;
                  						case 9:
                  								*(volatile unsigned char *)HEX0_BASE = 24;
                  								break;
                  						}
                }
/*
KEYが押された時に*(volatile unsigned char *)KEY_BASEに代入される値の関係
KEY0:14
KEY1:13
KEY2:11
KEY3:7
*/


                switch(*(volatile unsigned char *)KEY_BASE){
                //KEY0を押すと入力された数字を表示し、value_arrayに代入
                	case 14:
                		flag13 = false;
                		flag11 = false;
                		flag7 = false;
                		//バグ回避
                		if(value_count == 0 || value_count == 1)flag11 = true;

                		if(flag14 == false){
                			flag14 = true;
                			value_array[value_count]=value;
                			display_value(value_array[value_count]);
                			value_count++;
                		}
                        break;


                /*
                KEY1を押すと演算をどれにするかoperatorに格納
                0:加算
                1:減算
                2:乗算
                3:除算
                */

                	case 13:
                		flag14 = false;
                		flag11 = true;
                		flag7 = false;

                		if(flag13 == false){
                			flag13 = true;

                			switch (value) {
                				case 0:
                					operator[operator_count] = 0;
                          // +の表示
                					display_operator(0);
                					break;
                				case 1:
                					operator[operator_count] = 1;
                          // -の表示
                					display_operator(1);
                					break;

                				case 2:
                					operator[operator_count] = 2;
                          // *の表示
                					display_operator(2);
                					break;

                				case 3:
                					operator[operator_count] = 3;
                          // /の表示
                					display_operator(3);
                					break;
                			}
                			operator_count++;
                			if(operator_count > value_count){
                				reset();
                				reset();
                			}
                		}
            			break;



                /*
                KEY2を押すと計算が実行され、結果が表示される
                動作例）
                ディスプレイに「2 + 3 * 1 - 4」と出力された場合
                value_arrayには数値が格納される
                value_array[0]=2
                value_array[1]=3
                value_array[2]=1
                value_array[3]=4
                value_count = 4
                operatorには演算子番号が格納される
                operator[0]= 0
                operator[1]= 2
                operator[2]=1
                operator_count = 3
                がそれぞれ格納されている。
                まず1つ目のループでoperator[1]==2より
                value_array[1]*value_array[2]が実行され
                value_array[1]に3(3*1)が代入される。
                左詰め処理で
                value_array[0]=2
                value_array[1]=3
                value_array[2]=4
                value_count = 3
                operator[0]= 0
                operator[1]= 1
                operator_count = 2
                つまり「2 + 3 - 4」と同じ意味になる
                と動作していく。
                */
                case 11:
            		    flag14 = true;
            		    flag13 = true;
            		    flag7 = false;

            		    if(flag11 == false){
            			       flag11 = true;
                         //乗算除算を先に計算
            			       count=0;
            			       for(i=0;i<=operator_count;i++){
            			    	   if(count>10)break;
            			    	   if(operator_count==1)i=0;
                			       for(k=0;k<value_count;k++){
                			    	   printf("value_array[%d]=%d\n",k,value_array[k]);
                			       }
            			    	   printf("value_count=%d\n",value_count);
                			       for(k=0;k<operator_count;k++){
                			    	   printf("operator[%d]=%d\n",k,operator[k]);
                			       }
            			    	   printf("operator_count=%d\n",operator_count);
            			    	   printf("i=%d\n",i);
            			    	   printf("------------ */ --------------\n");
            			    	   if(operator[i]==2){
                             //計算し、左詰め
            			    		   value_array[i]=value_array[i]*value_array[i+1];
            			    		   for(j=i+1;j<value_count-1;j++){
            			    			   value_array[j]=value_array[j+1];
            			    		   }
            			    		   for(j=i;j<operator_count-1;j++){
            			    			   operator[j]=operator[j+1];
            			    		   }
            			    		   operator_count--;
            			    		   value_count--;
            			    		   i=0;
            			    	   }
            			    	   if(operator[i]==3){
            			    		   //計算し、左詰め
            			    		   operator[i]=operator[i+1];
            			    		   value_array[i]=value_array[i]/value_array[i+1];
            			    		   for(j=i+1;j<value_count-1;j++){
            			    			   value_array[j]=value_array[j+1];
            			    		   }
            			    		   for(j=i+1;j<operator_count-1;j++){
            			    			   operator[j]=operator[j+1];
            			    		   }
            			    		   operator_count--;
            			    		   value_count--;
            			    		   i=0;
            			    	   }
            			    	   count++;
            			       }
            			       //次に加算減算を計算
            			       for(i=0;i<=operator_count;i++){
            			    	   if(operator_count==1)i=0;
                			       for(k=0;k<value_count;k++){
                			    	   printf("value_array[%d]=%d\n",k,value_array[k]);
                			       }
            			    	   printf("value_count=%d\n",value_count);
                			       for(k=0;k<operator_count;k++){
                			    	   printf("operator[%d]=%d\n",k,operator[k]);
                			       }
            			    	   printf("operator_count=%d\n",operator_count);
            			    	   printf("i=%d\n",i);
            			    	   printf("------------ +- --------------\n");
            			    	   if(operator[i]==0){
            			    		   //計算し、左詰め
            			    		   operator[i]=operator[i+1];
            			    		   value_array[i]=value_array[i]+value_array[i+1];
            			    		   for(j=i+1;j<value_count-1;j++){
            			    			   value_array[j]=value_array[j+1];
            			    		   }
            			    		   for(j=i+1;j<operator_count-1;j++){
            			    			   operator[j]=operator[j+1];
            			    		   }
            			    		   operator_count--;
            			    		   value_count--;
            			    		   i=0;
            			    	   }
            			    	   if(operator[i]==1){
            			    		   //計算し、左詰め
            			    		   operator[i]=operator[i+1];
            			    		   value_array[i]=value_array[i]-value_array[i+1];
            			    		   for(j=i+1;j<value_count-1;j++){
            			    			   value_array[j]=value_array[j+1];
            			    		   }
            			    		   for(j=i+1;j<operator_count-1;j++){
            			    			   operator[j]=operator[j+1];
            			    		   }
            			    		   operator_count--;
            			    		   value_count--;
            			    		   i=0;
            			    	   }
            			       }
            			       for(k=0;k<value_count;k++){
            			    	   printf("value_array[%d]=%d\n",k,value_array[k]);
            			       }
        			    	   printf("value_count=%d\n",value_count);
            			       for(k=0;k<operator_count;k++){
            			    	   printf("operator[%d]=%d\n",k,operator[k]);
            			       }
        			    	   printf("operator_count=%d\n",operator_count);
        			    	   printf("i=%d\n",i);
        			    	   printf("------------end--------------\n");
                               display_operator(4);
                               display_value(value_array[0]);
                               value_count=0;
            		    }
            		    break;



                //KEY3が押されるとリセット
                        case 7:
                    		  flag14 = false;
                    		  flag13 = true;
                    		  flag11 = true;

                    		  if(flag7 == false){
                    			     flag7 = true;
                    			     reset();
                    			     reset();
                    		  }
                    		  break;
        }


        /*
        bool値確認↓(key3:key2:key1:key0)
        KEY操作のプログラムでは同じKEYが連続で押されることが無いようにしている
        それぞれのKEYにflagと呼ばれるブール値を代入させる変数を作っていて、
        flagがfalseのときだけそのKEYを押すことが出来る。
        動作例）
        KEY0が押された場合、KEY1,2,3のflagをfalseとし,KEY0のflagをtrueとしている。
        この動作をそれぞれのKEYで行っている。
        */
                //key3
                if(flag7 == true){
                    *(volatile unsigned char *)HEX7_BASE = 121;
                }else{
                    *(volatile unsigned char *)HEX7_BASE = 64;
                }
                //key2
                if(flag11 == true){
                    *(volatile unsigned char *)HEX6_BASE = 121;
                }else{
                    *(volatile unsigned char *)HEX6_BASE = 64;
                }
                //key1
                if(flag13 == true){
                    *(volatile unsigned char *)HEX5_BASE = 121;
                }else{
                    *(volatile unsigned char *)HEX5_BASE = 64;
                }
                //key0
                if(flag14 == true){
                    *(volatile unsigned char *)HEX4_BASE = 121;
                }else{
                    *(volatile unsigned char *)HEX4_BASE = 64;
                }
        }
    	              }
    	          }
    	}
        return 0;
}
void start(void){
  FILE *pLCD;

  char szHello[] = "1:calcurate game\n2:calcurator";


  pLCD = fopen(LCD_NAME, "w");

  if(pLCD){
          fwrite(szHello, strlen(szHello), 1, pLCD);
          fclose(pLCD);
  }else{
          printf("failed lcd");
  }

}

void congratulations(void){
  FILE *pLCD;

  char szHello[] = "  O";


  pLCD = fopen(LCD_NAME, "w");

  if(pLCD){
          fwrite(szHello, strlen(szHello), 1, pLCD);
          fclose(pLCD);
  }else{
          printf("failed lcd");
  }

}

void wrong(void){
  FILE *pLCD;

  char szHello[] = "  X";


  pLCD = fopen(LCD_NAME, "w");

  if(pLCD){
          fwrite(szHello, strlen(szHello), 1, pLCD);
          fclose(pLCD);
  }else{
          printf("failed lcd");
  }

}

//入力（演算子以外）をディスプレイに表示させる関数
void answer_is(void){
        FILE *pLCD;

        char szHello[] = "ans is";


        pLCD = fopen(LCD_NAME, "w");


        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }

}

//入力（演算子以外）をディスプレイに表示させる関数
void display_value(int fig){
        FILE *pLCD;

        char szHello[] = "\n";


        pLCD = fopen(LCD_NAME, "w");

        sprintf(szHello, "%d", fig);


        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }

}

//入力（演算子以外）をディスプレイに表示させる関数
void reset(void){
        FILE *pLCD;

        char szHello[] = "\n";


        pLCD = fopen(LCD_NAME, "w");

        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }

}
/*
演算子をディスプレイに表示する関数
display_operator(0)は+の表示
display_operator(1)は-の表示
display_operator(2)は*の表示
display_operator(3)/の表示
display_operator(4)は=の表示
*/
void display_operator(int operator){
        FILE *pLCD;

        if(operator == 0){
            pLCD = fopen(LCD_NAME, "w");

            if(pLCD){
                fwrite("+", 1, 1, pLCD);
                    fclose(pLCD);
            }else{
                    printf("failed lcd");
            }
        }
        else if(operator == 1){
            pLCD = fopen(LCD_NAME, "w");

            if(pLCD){
                    fwrite("-", 1, 1, pLCD);
                    fclose(pLCD);
            }else{
                    printf("failed lcd");
            }
        }
        else if(operator == 2){
            pLCD = fopen(LCD_NAME, "w");

            if(pLCD){
                fwrite("*", 1, 1, pLCD);
                    fclose(pLCD);
            }else{
                    printf("failed lcd");
            }
        }
        else if(operator == 3){
            pLCD = fopen(LCD_NAME, "w");

            if(pLCD){
                fwrite("/", 1, 1, pLCD);
                    fclose(pLCD);
            }else{
                    printf("failed lcd");
            }
        }
        else if(operator == 4){
            pLCD = fopen(LCD_NAME, "w");

            if(pLCD){
                fwrite("=", 1, 1, pLCD);
                    fclose(pLCD);
            }else{
                    printf("failed lcd");
            }
        }
}

