

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
#include "sys/alt_stdio.h"
#include <stdlib.h>
#include "system.h"
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include<math.h>
#include "altera_avalon_pio_regs.h"
#include "alt_types.h" //変数の型定義
#include "sys/alt_irq.h" //割込み用
#include "altera_avalon_timer_regs.h"

#define SIZE 256
#define NULL 0
/*
 speed:1　　　１秒ずつ
 speed:10　　　0.1秒ずつ

 */
#define SPEED 100
#define TIME_LIMIT 30*SPEED
#define COUNT_DAWN 5
#define GAME_NUMBER 4
#define SLOT_NUM 3
#define SLOT0_SPEED 0.8 
#define SLOT1_SPEED 0.5
#define SLOT2_SPEED 0.2
#define SLOT3_SPEED 0.1
#define SCORE_SPEED 3

volatile int n = 0;

void Timer_IRQ(void* fpVoid , alt_u32 fID)
{
    n++;
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
}

void Timer_Init(void)
{
    alt_u32 count;
    alt_irq_register(TIMER_IRQ,NULL,Timer_IRQ);

    count = 50000000/SPEED - 1;
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, count & 0xffff);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, (count >> 16) & 0xffff);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x07);
}



int main()
{
        int i,j,k,tmp;
        int count;
        int root[4];
        int slot_value[4];
        int answer,min,max,fig;
        int correct_answer;
        int amari;
        int key_value;
        int game_select_value;
        int random_operator,random_value[2];
        int value_count = 0,operator_count = 0;
        int operator[SIZE],value_array[SIZE];
        int value,past_score;
        int bango[GAME_NUMBER][SIZE],check_bango[GAME_NUMBER][SIZE],score[GAME_NUMBER][SIZE],your_score,play_num[GAME_NUMBER],player_num[GAME_NUMBER],clear_player[SIZE],rank_ray_clear[SIZE];
        int n1,n2,n_countdawn, n_sleep;
        int rank,status,rank_ray[GAME_NUMBER][SIZE];
        bool start_buf_flag = false;
        bool flag14 = false;
        bool flag13 = true;
        bool flag11 = true;
        bool flag7 = true;
        bool flag_start = false;
        bool flag_reset = false;
        bool flag_calcurate = false;
        bool flag_random = false;
        bool enter_bango_flag = false;
        bool time_limit_flag;
        bool switch_stop_flag = true;
        bool score_flag = false;
        bool correct_flag = true;
        int seed=0;

        //初期化
        for(j=0;j<GAME_NUMBER;j++){
        for(i=0;i<256;i++){
        	score[j][i]=999;
        	bango[j][i]=0;
        	check_bango[j][i]=0;

        	clear_player[i]=0;
        }
        	play_num[j]=0;
        	player_num[j]=0;
        }

        for(j=0;j<4;j++){
        	root[j]=0;
        	slot_value[j]=0;
        }
        for(i=0;i<256;i++){
        	score[3][i]=0;
        }


/*
     		if(flag == false){
			flag = true;
    		}
 */



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

    		  bool_check(flag7,flag11,flag13,flag14);

    		  //flag 初期化処理
    		if(flag_start == false){
    				flag_start = true;
    				flag_random = false;
    				time_limit_flag = false;
    				Timer_Init();
    				correct_flag = true;
    				min = 0;
    				max = SIZE;
    				count = 2;
    				game(0);
    				reset();
    				game(1);

    		}

            value = *(volatile unsigned char *)SWITCHES_BASE;
            *(volatile unsigned char *)LEDS_BASE = value;



                  seg_display(value);

    	          /*
    	          1か2をスイッチで選択し、KEY0を押すことでゲームを決定する
    	          1:calcurate game
    	          2:calcurator
    	          */
    	          if(*(volatile unsigned char *)KEY_BASE==13){
    	        	  reset();
    	        	  game(count%GAME_NUMBER);
    	        	  n_sleep = n;
    	        	  //0.5秒遅延
    	        	  while(n - n_sleep <= 1*SPEED/2){}
    	        	  count++;
    	          }
//-------------------------------------------------------------------------------------- calc game --------------------------------------------------------------------------------------------------
    	          if(*(volatile unsigned char *)KEY_BASE==14){
    	              game_select_value = *(volatile unsigned char *)SWITCHES_BASE;
    	              game_select_value = game_select_value % 4;
    	              if(game_select_value == 0){
    	          		if(flag_reset == false){
    	          				flag14 = true;
    	          				flag13 = false;
    	          				flag_reset = true;
    	          				reset();
    	          		}

    	          		//番号入力画面
    	          		while(1){

       	                  //calcurate game のループをbreakし、ゲーム選択画面に戻る
       	                  if(*(volatile unsigned char *)KEY_BASE==3){
       	                	flag_start = false;
       	                	flag_reset = false;
                  				flag_calcurate = false;
                  				start_buf_flag = false;
                  		        flag14 = false;
                  		        flag13 = false;
                  		        flag11 = true;
                  		        flag7 = true;
      	          				enter_bango_flag = false;
          	          			switch_stop_flag = true;

       	                	reset();
       	                    break;
       	                  }

    	          		  bool_check(flag7,flag11,flag13,flag14);
    	                    value = *(volatile unsigned char *)SWITCHES_BASE;
    	                    *(volatile unsigned char *)LEDS_BASE = value;

    	                    seg_display(value);

    	          			if(enter_bango_flag == false){
    	          				enter_bango_flag = true;
    	          				reset();
    	          				enter_bango();
    	          			}
    	      	          if(*(volatile unsigned char *)KEY_BASE==13){
  	          				flag14 = false;
  	          				flag13 = true;
    	      	              bango[game_select_value][play_num[game_select_value]] = *(volatile unsigned char *)SWITCHES_BASE;
    	      	              display_value(bango[game_select_value][play_num[game_select_value]]);
    	      	              reset();
    	      	              //bango の重複を確認
    	      	              for(i=0,j=0;i<play_num[game_select_value];i++){
    	      	            	  if(bango[game_select_value][play_num[game_select_value]] == bango[game_select_value][i]){
    	      	            		  j++;
    	      	            	  }
    	      	              }
              				play_num[game_select_value]++;

    	      	              //重複なしの場合
    	      	              if(j==0){
    	      	            	  player_num[game_select_value]++;

    	      	              }
    	      	              else{
    	      	              }
    	      	            n1 = n;
    	      	                   	      	              time_limit_flag = false;
    	      	                   	      	              past_score = score[game_select_value][bango[game_select_value][play_num[game_select_value]]];
    	      	                   	      	              score[game_select_value][bango[game_select_value][play_num[game_select_value]]]= 0;

    	      	                   	      	              break;
    	      	          }
    	          		}
    	          		//finish 番号入力画面




    	          		//カウントダウン
    	          		i=0;
    	          		n_countdawn = n/SPEED;
                		while(n/SPEED - n_countdawn < COUNT_DAWN + 1){

         	                  //calcurate game のループをbreakし、ゲーム選択画面に戻る
         	                  if(*(volatile unsigned char *)KEY_BASE==3){
         	                    break;
         	                  }

                			if((n/SPEED - n_countdawn + i) % 2 == 1){
                			display_value(COUNT_DAWN + 1 - (n/SPEED - n_countdawn));
                			kuhaku();
                			i++;
                			}
                		}
                		display_start();
                		reset();
                		//finish カウントダウン

    	                while (1) {

    	                	  bool_check(flag7,flag11,flag13,flag14);

    	                  //time limit
    	               		if(time_limit_flag == false){
    	                	if(n - n1 > TIME_LIMIT){
        	          			time_limit_flag = true;
        	          			switch_stop_flag = false;
        	          			reset();
    	                		time_limit();
    	                		reset();
    	                		flag_calcurate = true;
    	                		n2 = n;
    	                		while(n - n2 < 3*SPEED){
    	                			if(score_flag ==false){
    	                				score_flag = true;
    	                				your_score = score[game_select_value][bango[game_select_value][play_num[game_select_value]]];
    	                				display_score(your_score);
    	                				reset();

    	                				  for (i=0; i<SIZE; ++i) {
    	                					  if(i == bango[game_select_value][play_num[game_select_value]] && score[game_select_value][bango[game_select_value][play_num[game_select_value]]] < past_score){
    	                					  }
    	                					  else{
    	                					  rank_ray[game_select_value][i] = score[game_select_value][i];
    	                				  }
    	                				  }
  	                				  //sort
  	                				  for (i=0; i<SIZE; ++i) {
  	                				    for (j=i+1; j<SIZE; ++j) {
  	                				      if (rank_ray[game_select_value][i] < rank_ray[game_select_value][j]) {
  	                				        tmp =  rank_ray[game_select_value][i];
  	                				        rank_ray[game_select_value][i] = rank_ray[game_select_value][j];
  	                				        rank_ray[game_select_value][j] = tmp;
  	                				      }
  	                				    }
  	                				  }//finish sort

  	                				  //score display
	            	  				  printf("-------- 計算ゲーム　ランキング --------\n");
  	                				  for (i=0,k=0; i<SIZE; i++) {
  	                					  if(rank_ray[game_select_value][i] != 999 && rank_ray[game_select_value][i] != rank_ray[game_select_value][i-1]){
  	                						  k++;
  	  	                				  for (j=0; j<SIZE; j++) {
  	  	                					  if(rank_ray[game_select_value][i] == score[game_select_value][j]){
  	  	                						  status = j;
  	  	                						  printf("%d位 番号:%d スコア:%d",k,j,score[game_select_value][j]);
  	  	                						  if(j == bango[game_select_value][play_num[game_select_value]]){
  	  	                							  printf(" ←　");
  	  	                							  rank = k;
  	  	                						  }
	  	                							  printf("\n");
  	  	                					  }
  	  	                				  }
  	                					  }
  	                				  }
  	                				  printf("-------------------------------------------------\n");
    	                				display_rank(rank);
    	                				display_value(player_num[game_select_value]);
    	                			}
    	                		}
                				score_flag = false;
        	          	        flag14 = true;
        	          	        flag13 = true;
        	          	        flag11 = true;
        	          	        flag7 = true;
    	                	}
    	              		}
    	               		//finish time limit



    	    	              value = *(volatile unsigned char *)SWITCHES_BASE;
    	    	              seg_display(value);

          	          		if(flag_calcurate == false){
          	          				flag_calcurate = true;
          	          				for(i=0;i<10;i++){

          	          				//rand()%(B-A+1)+A; AからBまでのランダム整数
          	          				if(flag_random == false){
          	          					flag_random = true;
          	          					//srand(n);
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

      	          				//printf("correct_ans=%d\n",correct_answer);
      	          		//}
      	          				if(correct_answer > 0 && correct_answer < 255){break;}
      	          				else{
      	          					//printf("miss\n");
      	          				}
          	          		}
              	                  //画面表示
                	          				display_value(random_value[1]);
                	          				display_operator(random_operator);
                	          				display_value(random_value[0]);
                	          				display_operator(4);
          	          		}


          	          		if(switch_stop_flag == true){
    	                  switch(*(volatile unsigned char *)KEY_BASE){
    	                   //KEY0を押すと入力された数字を表示し、value_arrayに代入
    	                   	case 14:
    	                   		flag13 = false;
    	                   		flag11 = false;
    	                   		flag7 = false;
	                   			if(start_buf_flag == true){
    	                   		if(flag14 == false){
    	                   			flag14 = true;
    	                   				answer = value;
    	                   				display_value(answer);
    	                   				if(answer == correct_answer){
    	                   					good();
    	                   					score[game_select_value][bango[game_select_value][play_num[game_select_value]]]++;
    	                   				}
    	                   				else{
    	                   					wrong();
    	                   				}

    	                   				flag_calcurate = false;
    	                   			reset();
    	                   			}
	                   			}else{
	                   				start_buf_flag = true;
	                   			}
    	                           break;


    	                   /*
    	                   KEY1を押すと演算をどれにするかoperatorに格納
    	                   */

    	                   	case 13:
    	                   		flag14 = false;
    	                   		flag11 = false;
    	                   		flag7 = false;
	                   			if(start_buf_flag == true){
    	                   		if(flag13 == false){
    	                   			flag13 = true;
    	                   			answer = value;
       	                   			display_value(answer);
	                   				if(answer == correct_answer){
	                   					good();
	                   					score[game_select_value][bango[game_select_value][play_num[game_select_value]]]++;
	                   				}
	                   				else{
	                   					wrong();
	                   				}

	                   				flag_calcurate = false;
        	                   			reset();
    	                   		}
	                   			}else{
	                   				start_buf_flag = true;
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




    	                }//switch key_base
          	          		}

     	                  //calcurate game のループをbreakし、ゲーム選択画面に戻る
     	                  if(*(volatile unsigned char *)KEY_BASE==3){
     	                	flag_start = false;
     	                	flag_reset = false;
                				flag_calcurate = false;
                				start_buf_flag = false;
                		        flag14 = false;
                		        flag13 = true;
                		        flag11 = true;
                		        flag7 = true;
    	          				enter_bango_flag = false;
        	          			switch_stop_flag = true;

     	                	reset();
     	                    break;
     	                  }

    	              }//calc_game loop
    	              }
   //----------------------------------------------------------------------------------------- calcurator -------------------------------------------------------------------------------------------
    	              else if(game_select_value == 1){
      	          		if(flag_reset == false){
      	       		        flag14 = true;
      	       		        flag13 = true;
      	       		        flag11 = true;
      	       		        flag7 = false;
      	          				flag_reset = true;
      	          				reset();
      	          		}
      	          		//calcurator start
        while(1){
            //スイッチとLEDを連動させている
                value = *(volatile unsigned char *)SWITCHES_BASE;
                *(volatile unsigned char *)LEDS_BASE = value;

                if(*(volatile unsigned char *)KEY_BASE==3){
                	flag_start = false;
                	flag_reset = false;
       				start_buf_flag = false;
       		        flag14 = false;
       		        flag13 = false;
       		        flag11 = true;
       		        flag7 = true;
                	reset();
                	reset();
          			switch_stop_flag = true;
                  break;
                }


                seg_display(value);

                /*
                 00:4で割り切れる　かつ　偶数
                 01:4で割ると1余る奇数(9,25,41,57,)
                 10:4で割ると2余る偶数
                 11:4で割ると3余る奇数(11,19,43,51,75)
                 */

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

                		//バグ回避
                		if(value_count == 0 || value_count == 1)flag11 = true;

            			if(start_buf_flag == true){
                		if(flag14 == false){
                       		flag13 = false;
                        		flag11 = false;
                        		flag7 = false;
                			flag14 = true;
                				value_array[value_count]=value;
                				display_value(value_array[value_count]);
                				value_count++;
                		}
            			}else{
               				start_buf_flag = true;
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


                		if(flag13 == false){
                    		flag14 = false;
                    		flag11 = true;
                    		flag7 = false;
                			flag13 = true;

                			switch (value%4) {
                				case 0:
                					operator[operator_count] = 0;
                          // +の表示
                					display_operator(0);
                					operator_count++;
                					break;
                				case 1:
                					operator[operator_count] = 1;
                          // -の表示
                					display_operator(1);
                					operator_count++;
                					break;

                				case 2:
                					operator[operator_count] = 2;
                          // *の表示
                					display_operator(2);
                					operator_count++;
                					break;

                				case 3:
                					operator[operator_count] = 3;
                          // /の表示
                					display_operator(3);
                					operator_count++;
                					break;

                				default:
                					flag14 = true;
                            		flag13 = false;
                            		flag11 = false;
                            		flag7 = false;
                					break;
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

            		    if(flag11 == false){
                		    flag14 = true;
                		    flag13 = true;
                		    flag7 = false;
            			       flag11 = true;
                         //乗算除算を先に計算
            			       count=0;
            			       for(i=0;i<=operator_count;i++){
            			    	   //if(count>SIZE)break;
              			    	   if(operator_count==0)break;
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
            			    	   //count++;
            			       }
            			       //次に加算減算を計算
            			       for(i=0;i<=operator_count;i++){
            			    	   i=0;
            			    	   if(operator_count==0)break;
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
                               reset();
        			    	   display_operator(4);
                               display_value(value_array[0]);
                               value_count=0;
            		    }
            		    break;



                //KEY3が押されるとリセット
                        case 7:


                    		  if(flag7 == false){
                        		  flag14 = false;
                        		  flag13 = true;
                        		  flag11 = true;
                    			     flag7 = true;
                    			     reset();
                    			     reset();
                                     value_count= 0;
                                     operator_count = 0;
                    		  }
                    		  break;
        }


  bool_check(flag7,flag11,flag13,flag14);
        }    	            	  //finish calcurator

    	              }
 //------------------------------------------------------------------------------------ numbers game -------------------------------------------------------------------------------------------------
    	              else if(game_select_value == 2){
    	            	  	if(flag_reset == false){
    	            	  			flag14 = true;
    	            	  			flag13 = false;
    	            	  			flag_reset = true;
    	            	  			reset();
    	            	  	}

    	            	  	//番号入力画面
    	            	  	while(1){

    	            	       //calcurate game のループをbreakし、ゲーム選択画面に戻る
    	            	       if(*(volatile unsigned char *)KEY_BASE==3){
    	            	     	flag_start = false;
    	            	     	flag_reset = false;
    	            	  			flag_calcurate = false;
    	            	  			start_buf_flag = false;
    	            	  	        flag14 = false;
    	            	  	        flag13 = false;
    	            	  	        flag11 = true;
    	            	  	        flag7 = true;
    	            	  			enter_bango_flag = false;
    	            	  			switch_stop_flag = true;

    	            	     	reset();
    	            	         break;
    	            	       }

    	            	  	  bool_check(flag7,flag11,flag13,flag14);
    	            	      value = *(volatile unsigned char *)SWITCHES_BASE;
    	            	      seg_display(value);

    	            	  		if(enter_bango_flag == false){
    	            	  			enter_bango_flag = true;
    	            	  			reset();
    	            	  			enter_bango();
    	            	  		}
    	            	      if(*(volatile unsigned char *)KEY_BASE==13){
    	            	  		flag14 = false;
    	            	  		flag13 = true;
    	            	          bango[game_select_value][play_num[game_select_value]] = *(volatile unsigned char *)SWITCHES_BASE;
    	            	          display_value(bango[game_select_value][play_num[game_select_value]]);
    	            	          reset();
    	            	          reset();
    	            	          //bango の重複を確認
    	            	          for(i=0,j=0;i<play_num[game_select_value];i++){
    	            	        	  if(bango[game_select_value][play_num[game_select_value]] == bango[game_select_value][i]){
    	            	        		  j++;
    	            	        	  }
    	            	          }
	            	  				play_num[game_select_value]++;

    	            	          //重複なしの場合
if(j==0){
    	            	        	  player_num[game_select_value]++;

}else{

}
n1 = n;
   	            	              time_limit_flag = false;
   	            	              past_score = score[game_select_value][bango[game_select_value][play_num[game_select_value]]];
   	            	              score[game_select_value][bango[game_select_value][play_num[game_select_value]]] = 0;
   	            	              break;

    	            	      }
    	            	  	}
    	            	  	//finish 番号入力画面



    	            	  	correct_answer = rand()%255 + 1;
      	  				what_is_answer();
      	  				reset();

    	            	  while (1) {

    	            	  	  bool_check(flag7,flag11,flag13,flag14);

    	            	    //time limit
    	            	  		if(time_limit_flag == false){
    	            	  	if(correct_flag == false){
    	            	  		correct_flag = true;
    	            	  			time_limit_flag = true;
    	            	  			switch_stop_flag = false;
    	            	  			reset();
    	            	  		time_limit();
    	            	  		reset();
    	            	  		flag_calcurate = true;
    	            	  		n2 = n;
    	            	  		while(n - n2 < 3*SPEED){
    	            	  			if(score_flag ==false){
    	            	  				score_flag = true;
    	            	  				your_score = score[game_select_value][bango[game_select_value][play_num[game_select_value]]];
    	            	  				display_score(your_score);
    	            	  				reset();

    	            	  				for (i=0; i<SIZE; ++i) {
    	            	  					if(i == bango[game_select_value][play_num[game_select_value]] && score[game_select_value][bango[game_select_value][play_num[game_select_value]]] > past_score){
    	            	  					}
    	            	  					else{
    	            	  						rank_ray[game_select_value][i] = score[game_select_value][i];
    	            	  					}
    	            	  				}

    	            	  				  //sort(昇順)
    	            	  				  for (i=0; i<SIZE; ++i) {
    	            	  				    for (j=i+1; j<SIZE; ++j) {
    	            	  				      if (rank_ray[game_select_value][i] > rank_ray[game_select_value][j]) {
    	            	  				        tmp =  rank_ray[game_select_value][i];
    	            	  				        rank_ray[game_select_value][i] = rank_ray[game_select_value][j];
    	            	  				        rank_ray[game_select_value][j] = tmp;
    	            	  				      }
    	            	  				    }
    	            	  				  }//finish sort

    	            	  				  //score display]
    	            	  				  printf("-------- 数合わせゲーム　ランキング --------\n");
    	            	  				  for (i=0,k=0; i<SIZE; i++) {
    	            	  					  if(rank_ray[game_select_value][i] != 999 && rank_ray[game_select_value][i] != rank_ray[game_select_value][i-1]){
    	            	  						  k++;
    	            	      				  for (j=0; j<SIZE; j++) {
    	            	      					  if(rank_ray[game_select_value][i] == score[game_select_value][j]){
    	            	      						  status = j;
    	            	      						  printf("%d位 番号:%d スコア:%d",k,j,score[game_select_value][j]);
    	            	      						  if(j == bango[game_select_value][play_num[game_select_value]]){
    	            	      							  printf(" ←　");
    	            	      							  rank = k;
    	            	      						  }
    	            	    							  printf("\n");
    	            	      					  }
    	            	      				  }
    	            	  					  }
    	            	  				  }
    	            	  				  printf("-------------------------------------------------\n");
    	            	  				display_rank(rank);
    	            	  				display_value(player_num[game_select_value]);
    	            	  			}
    	            	  		}
    	            	  		score_flag = false;
    	            	  	        flag14 = true;
    	            	  	        flag13 = true;
    	            	  	        flag11 = true;
    	            	  	        flag7 = true;
    	            	  	}
    	            	  		}
    	            	  		//finish time limit



    	            	        value = *(volatile unsigned char *)SWITCHES_BASE;
    	            	        seg_display(value);

    	            	  		if(flag_calcurate == false){
    	            	  				flag_calcurate = true;
    	            	  				//rand()%(B-A+1)+A; AからBまでのランダム整数
    	            	  				if(flag_random == false){
    	            	  					flag_random = true;
    	            						srand(n);
    	            	  				}



	            	     				score[game_select_value][bango[game_select_value][play_num[game_select_value]]]++;
    	            	              //画面表示
    	            	  				//what_is_answer();

    	            	  		}//画面表示　if


    	            	  		if(switch_stop_flag == true){
    	            	    switch(*(volatile unsigned char *)KEY_BASE){
    	            	     //KEY0を押すと入力された数字を表示し、value_arrayに代入
    	            	     	case 14:
    	            	     		flag13 = false;
    	            	     		flag11 = false;
    	            	     		flag7 = false;
    	            	  			if(start_buf_flag == true){
    	            	     		if(flag14 == false){
    	            	     			flag14 = true;
    	            	     				answer = value;
    	            	     				display_value(answer);
    	            	     				if(answer > correct_answer){
    	            	     					fig = 0;
    	            	     					max = answer;
    	            	     				}
    	            	     				else if(answer < correct_answer){
    	            	     					fig = 1;
    	            	     					min = answer;
    	            	     				}
    	            	     				else if(answer == correct_answer){
    	            	     					fig = 2;
    	            	     					correct_flag = false;

    	            	     				}
    	            	     				smaller_or_bigger(fig);

    	            	     				if(fig != 2){
    	            	     					past(min,max);
    	            	     				}
    	            	        			else{
    	            	        				//1秒遅延
    	            	        				n_sleep = n;
    	            	        				while(n - n_sleep <= 1*SPEED){}
    	            	        			}

    	            	     				flag_calcurate = false;
    	            	     			}
    	            	  			}else{
    	            	  				start_buf_flag = true;
    	            	  			}
    	            	             break;


    	            	     /*
    	            	     KEY1を押すと演算をどれにするかoperatorに格納
    	            	     */

    	            	     	case 13:
    	            	     		flag14 = false;
    	            	     		flag11 = false;
    	            	     		flag7 = false;
    	            	  			if(start_buf_flag == true){
    	            	     		if(flag13 == false){
    	            	     			flag13 = true;
    	            	     			answer = value;
    	            	        			display_value(answer);
    	            	        			if(answer > correct_answer){
    	            	        				fig = 0;
    	            	        				max = answer;
    	            	        			}
    	            	        			else if(answer < correct_answer){
    	            	        				fig = 1;
    	            	        				min = answer;
    	            	        			}
    	            	        			else if(answer == correct_answer){
    	            	        				fig = 2;
    	            	        				correct_flag = false;

    	            	        			}
    	            	        			smaller_or_bigger(fig);
    	            	        			if(fig != 2){
    	            	        				past(min,max);
    	            	        			}
    	            	        			else{
    	            	        				//1秒遅延
    	            	        				n_sleep = n;
    	            	        				while(n - n_sleep <= 1*SPEED){}
    	            	        			}

    	            	  				flag_calcurate = false;
    	            	     		}
    	            	  			}else{
    	            	  				start_buf_flag = true;
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




    	            	  }//switch key_base
    	            	  		}

    	            	     //numbers game のループをbreakし、ゲーム選択画面に戻る
    	            	     if(*(volatile unsigned char *)KEY_BASE==3){
    	            	   	flag_start = false;
    	            	   	flag_reset = false;
    	            	  		flag_calcurate = false;
    	            	  		start_buf_flag = false;
    	            	          flag14 = false;
    	            	          flag13 = true;
    	            	          flag11 = true;
    	            	          flag7 = true;
    	            	  			enter_bango_flag = false;
    	            	  			switch_stop_flag = true;

    	            	   	reset();
    	            	       break;
    	            	     }

    	            	  }//main game loop
    	              }//numbers game loop
    //------------------------------------------------------------------------------------ slot -------------------------------------------------------------------------------------------------
    	          else if(game_select_value % 4 == 3){


    	        	  	  flag14 = false;
    	        	  	  flag13 = false;
    	        	  	  flag11 = false;
    	        	  	  flag7 = false;

  	            	  	//番号入力画面
  	            	  	while(1){

  	            	       //calcurate game のループをbreakし、ゲーム選択画面に戻る
  	            	       if(*(volatile unsigned char *)KEY_BASE==3){
  	            	     	flag_start = false;
  	            	     	flag_reset = false;
  	            	  			flag_calcurate = false;
  	            	  			start_buf_flag = false;
  	            	  	        flag14 = false;
  	            	  	        flag13 = false;
  	            	  	        flag11 = true;
  	            	  	        flag7 = true;
  	            	  			enter_bango_flag = false;
  	            	  			switch_stop_flag = true;

  	            	     	reset();
  	            	         break;
  	            	       }

  	            	  	  bool_check(flag7,flag11,flag13,flag14);
  	            	      value = *(volatile unsigned char *)SWITCHES_BASE;
  	                    *(volatile unsigned char *)LEDS_BASE = value;

  	            	      seg_display(value);

  	            	  		if(enter_bango_flag == false){
  	            	  			enter_bango_flag = true;
  	            	  			reset();
  	            	  			enter_bango();
  	            	  		}
  	            	      if(*(volatile unsigned char *)KEY_BASE==13){

  	    	            	  //1秒遅延
  	    	            	   n_sleep = n;
  	    	            	   while(n - n_sleep <= 1*SPEED){}

  	            	          bango[game_select_value][play_num[game_select_value]] = *(volatile unsigned char *)SWITCHES_BASE;
  	            	          display_value(bango[game_select_value][play_num[game_select_value]]);

  	            	          reset();
  	            	          fight();
  	            	          //bango の重複を確認
  	            	          for(i=0,j=0;i<play_num[game_select_value];i++){
  	            	        	  if(bango[game_select_value][play_num[game_select_value]] == bango[game_select_value][i]){
  	            	        		  j++;
  	            	        	  }
  	            	          }
  	            	          play_num[game_select_value]++;
  	            	          //重複なしの場合
  	            	          if(j==0){
  	            	        	  player_num[game_select_value]++;
  	            	          }else{}
  	            	          n1 = n;
  	            	          time_limit_flag = false;
  	            	          score[game_select_value][bango[game_select_value][play_num[game_select_value]]]++;
            	              break;

  	            	      }
  	            	  	}
  	            	  	//finish 番号入力画面



    	        	  while(1){


	            	  	  bool_check(flag7,flag11,flag13,flag14);

	            	    //time limit
	            	  	if(correct_flag == false){
	            	  			correct_flag = true;
	            	  			time_limit_flag = true;
	            	  			switch_stop_flag = false;
	            	  			reset();
	            	  		flag_calcurate = true;
	            	  		n2 = n;
	            	  		while(n - n2 < 3*SPEED){
	            	  			if(score_flag ==false){
	            	  				score_flag = true;
	            	  				your_score = score[game_select_value][bango[game_select_value][play_num[game_select_value]]];
	            	  				display_score(your_score);
	            	  				reset();

	            	  				  for (i=0; i<SIZE; ++i) {
	            	  					  rank_ray[game_select_value][i] = score[game_select_value][i];
	            	  					  rank_ray_clear[i] = clear_player[i];
	            	  				  }
	            	  				  //sort(降順) 挑戦者
	            	  				  for (i=0; i<SIZE; ++i) {
	            	  				    for (j=i+1; j<SIZE; ++j) {
	            	  				      if (rank_ray[game_select_value][i] < rank_ray[game_select_value][j]) {
	            	  				        tmp =  rank_ray[game_select_value][i];
	            	  				        rank_ray[game_select_value][i] = rank_ray[game_select_value][j];
	            	  				        rank_ray[game_select_value][j] = tmp;
	            	  				      }
	            	  				    }
	            	  				  }//finish sort

	            	  				  //sort(昇順) クリア者
	            	  				  for (i=0; i<SIZE; ++i) {
	            	  				    for (j=i+1; j<SIZE; ++j) {
	            	  				      if (rank_ray_clear[i] < rank_ray_clear[j]) {
	            	  				        tmp =  rank_ray_clear[i];
	            	  				        rank_ray_clear[i] = rank_ray_clear[j];
	            	  				        rank_ray_clear[j] = tmp;
	            	  				      }
	            	  				    }
	            	  				  }//finish sort

	            	  				  //score display 挑戦者
	            	  				  printf("-------- スロットゲーム　挑戦者  頑張りランキング　--------\n");
	            	  				  for (i=0,k=0; i<SIZE; i++) {
	            	  					  if(rank_ray[game_select_value][i] != 0 && rank_ray[game_select_value][i] != rank_ray[game_select_value][i-1]){
	            	  						  k++;
	            	      				  for (j=0; j<SIZE; j++) {
	            	      					  if(rank_ray[game_select_value][i] == score[game_select_value][j]){
	            	      						  status = j;
	            	      						  printf("%d位 番号:%d 総挑戦回数:%d",k,j,score[game_select_value][j]);
	            	      						  if(j == bango[game_select_value][play_num[game_select_value]]){
	            	      							  printf(" ←　");
	            	      							  rank = k;
	            	      						  }
	            	    							  printf("\n");
	            	      					  }
	            	      				  }
	            	  					  }
	            	  				  }

	            	  				  //score display クリア者
	            	  				  printf("-------- スロットゲーム　クリア者	 ランキング　--------\n");
	            	  				  for (i=0,k=0; i<SIZE; i++) {
	            	  					  if(rank_ray_clear[i] != 0 && rank_ray_clear[i] != rank_ray_clear[i-1]){
	            	  						  k++;
	            	      				  for (j=0; j<SIZE; j++) {
	            	      					  if(rank_ray_clear[i] == clear_player[j]){
	            	      						  status = j;
	            	      						  printf("%d位 番号:%d クリアまでにかかった挑戦回数:%d",k,j,clear_player[j]);
	            	      						  if(j == bango[game_select_value][play_num[game_select_value]]){
	            	      							  printf(" ←　");
	            	      							  rank = k;
	            	      						  }
	            	    							  printf("\n");
	            	      					  }
	            	      				  }
	            	  					  }
	            	  				  }
	    							  printf("-------------------------------------------------------------------------------------------------\n");
	            	  				display_rank(rank);
	            	  				display_value(player_num[game_select_value]);
	            	  			}
	            	  		}
	            	  	        flag14 = true;
	            	  	        flag13 = true;
	            	  	        flag11 = true;
	            	  	        flag7 = true;
	            	  	        while(1){
	         	            	     if(*(volatile unsigned char *)KEY_BASE==3){
	         	            	    	 break;
	         	            	     }
	            	  	        }
	            	  	}
	            	  		//finish time limit


	            	  		if(flag_calcurate == false){
	            	  				flag_calcurate = true;
	            	  				//rand()%(B-A+1)+A; AからBまでのランダム整数
	            	  				if(flag_random == false){
	            	  					flag_random = true;
	            						srand(n);
	            	  				}



	            	              //画面表示
	            	  				//what_is_answer();

	            	  		}//画面表示　if





	          	            	  	  if(flag7 == false && n - root[3] > SPEED*SLOT3_SPEED){
	          	            	  	  	  slot_value[3]++;
	          	            	  	  	  root[3] = n;
	          	            	  	  	  slot(3,slot_value[3]);
	          	            	  	  }
	          	            	  	 if(flag11 == false && n - root[2] > SPEED*SLOT2_SPEED){
	          	            	  		 slot_value[2]++;
	          	            	  		 root[2] = n;
	          	            	  		 slot(2,slot_value[2]);
	          	            	  	 }
	          	            	  	 if(flag13 == false && n - root[1] > SPEED*SLOT1_SPEED){
	          	            	  		 slot_value[1]++;
	          	            	  		 root[1] = n;
	          	            	  		 slot(1,slot_value[1]);
	          	            	  	 }
	          	            	  	 if(flag14 == false && n - root[0] > SPEED*SLOT0_SPEED){
	         	            	  	  	  slot_value[0]++;
	         	            	  	  	  root[0] = n;
	         	            	  	  	  slot(0,slot_value[0]);
		          	            	 }

	        	            	     if(*(volatile unsigned char *)KEY_BASE==14){
	        	            	    	 flag14 = true;
	        	            	     }
	        	            	     if(*(volatile unsigned char *)KEY_BASE==13){
	        	            	    	 flag13 = true;
	        	            	     }
	        	            	     if(*(volatile unsigned char *)KEY_BASE==11){
	        	            	    	 flag11 = true;
	        	            	     }
	        	            	     if(*(volatile unsigned char *)KEY_BASE==7){
	        	            	    	 flag7 = true;
	        	            	     }

	        	            	     if(flag14==true && flag13==true && flag11==true && flag7==true){
	        	            	    	 k = slot_value[0]%10;

	        	            	    	 if(slot_value[1]%10==k && slot_value[2]%10==k && slot_value[3]%10==k){
	        	            	    		 if(check_bango[game_select_value][play_num[game_select_value]] == 0){
	        	            	    			 clear_player[bango[game_select_value][play_num[game_select_value]]] = score[game_select_value][bango[game_select_value][play_num[game_select_value]]];
	        	            	    			 check_bango[game_select_value][play_num[game_select_value]] = 1;
	        	            	    			 //score[game_select_value][bango[game_select_value][play_num[game_select_value]]] = 0;
	        	            	    			 reset();
	        	            	    			 cong();
	        	            	    			  //1秒遅延
	        	            	    			  	    	            	   n_sleep = n;
	        	            	    			  	    	            	   while(n - n_sleep <= 1*SPEED){}
	        	            	    	 }
	        	            	    	 }
	        	            	    	 correct_flag = false;
	        	            	    	 score_flag = false;
	        	            	     }



    	        	            	     //numbers game のループをbreakし、ゲーム選択画面に戻る
    	         	            	     if(*(volatile unsigned char *)KEY_BASE==3){
    	         	            	   	flag_start = false;
    	         	            	   	flag_reset = false;
    	         	            	  		flag_calcurate = false;
    	         	            	  		start_buf_flag = false;
    	         	            	          flag14 = false;
    	         	            	          flag13 = true;
    	         	            	          flag11 = true;
    	         	            	          flag7 = true;
    	         	            	  			enter_bango_flag = false;
    	         	            	  			switch_stop_flag = true;

    	         	            	  			reset();
    	         	            	  			break;
    	         	            	     }
    	          	            	  }//main game

    	          	              }
//決定ボタン if
    	          }
    	}//全体のwhile
        return 0;
}


//------------------------------------------------------------------------------------ function -------------------------------------------------------------------------------------------------

//start 画面　表示
void game(int x){
  FILE *pLCD;

  char a[] = "0:calcurate game";
  char b[] = "1:calcurator";
  char c[] = "2:numbers game";
  char d[] = "3:slot";




  pLCD = fopen(LCD_NAME, "w");
if(x==0){
  if(pLCD){
          fwrite(a, strlen(a), 1, pLCD);
          fclose(pLCD);
  }else{
          printf("failed lcd");
  }
}
else if(x==1){
  if(pLCD){
          fwrite(b, strlen(b), 1, pLCD);
          fclose(pLCD);
  }else{
          printf("failed lcd");
  }
}
else if(x==2){
  if(pLCD){
          fwrite(c, strlen(c), 1, pLCD);
          fclose(pLCD);
  }else{
          printf("failed lcd");
  }
}
else if(x==3){
  if(pLCD){
          fwrite(d, strlen(d), 1, pLCD);
          fclose(pLCD);
  }else{
          printf("failed lcd");
  }
}

}

void what_is_answer(void){
        FILE *pLCD;

        char szHello[] = "what is answer?\n";


        pLCD = fopen(LCD_NAME, "w");

        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }
}

void smaller_or_bigger(int fig){
        FILE *pLCD;

        char a[] = " dawn";
        char b[] = " up";
        char c[] = " correct!!";




        pLCD = fopen(LCD_NAME, "w");

if(fig == 0){
        if(pLCD){
                fwrite(a, strlen(a), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }
}
else if(fig == 1){
    if(pLCD){
            fwrite(b, strlen(b), 1, pLCD);
            fclose(pLCD);
    }else{
            printf("failed lcd");
    }
}
else if(fig == 2){
    if(pLCD){
    	//printf("correct!\n");
            fwrite(c, strlen(c), 1, pLCD);
            fclose(pLCD);
    }else{
            printf("failed lcd");
    }
}
}


//正解 判定
void good(void){
        FILE *pLCD;

        char szHello[] = " O";


        pLCD = fopen(LCD_NAME, "w");

        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }
}

void fight(void){
        FILE *pLCD;

        char szHello[] = "FIGHT!";


        pLCD = fopen(LCD_NAME, "w");

        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }
}
void cong(void){
        FILE *pLCD;

        char szHello[] = "congulatuation!";


        pLCD = fopen(LCD_NAME, "w");

        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }
}

void past(int min,int max){
                FILE *pLCD;
                char c[] = " ";
                char a[] = "\n\n\n";
                char b[] = "<";
                char d[] = " \n";
                char e[] = "X";


                pLCD = fopen(LCD_NAME, "w");

                if(pLCD){
                			fwrite(c, strlen(c), 1, pLCD);

                            sprintf(a, "%d", min);
                            fwrite(a, strlen(a), 1, pLCD);

                        	fwrite(b, strlen(b), 1, pLCD);

                        	fwrite(e, strlen(e), 1, pLCD);

                        	fwrite(b, strlen(b), 1, pLCD);

                            sprintf(a, "%d", max);
                            fwrite(a, strlen(a), 1, pLCD);

                            fwrite(d, strlen(d), 1, pLCD);

                        fclose(pLCD);
                }else{
                        printf("failed lcd");
                }
}

void wrong(void){
    FILE *pLCD;

    char szHello[] = " X";


    pLCD = fopen(LCD_NAME, "w");

    if(pLCD){
            fwrite(szHello, strlen(szHello), 1, pLCD);
            fclose(pLCD);
    }else{
            printf("failed lcd");
    }

}

//入力（演算子以外）をディスプレイに表示させる関数
void time_limit(void){
        FILE *pLCD;

        char szHello[] = "finished!";


        pLCD = fopen(LCD_NAME, "w");


        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }

}

void display_rank(int rank){
    FILE *pLCD;

    char szHello[] = "rank: ";
    char in[] = " in ";

    pLCD = fopen(LCD_NAME, "w");


    if(pLCD){
            fwrite(szHello, strlen(szHello), 1, pLCD);
            sprintf(szHello, "%d", rank);
            fwrite(szHello, strlen(szHello), 1, pLCD);
            fwrite(in, strlen(in), 1, pLCD);

            fclose(pLCD);
    }else{
            printf("failed lcd");
    }
}

void display_score(int score){
    FILE *pLCD;

    char szHello[] = "your score: ";


    pLCD = fopen(LCD_NAME, "w");

    if(pLCD){
            fwrite(szHello, strlen(szHello), 1, pLCD);
            sprintf(szHello, "%d", score);
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
void kuhaku(void){
        FILE *pLCD;

        char szHello[] = " ";


        pLCD = fopen(LCD_NAME, "w");

        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }

}
//入力（演算子以外）をディスプレイに表示させる関数
void display_start(void){
        FILE *pLCD;

        char szHello[] = "start!";


        pLCD = fopen(LCD_NAME, "w");

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

//入力（演算子以外）をディスプレイに表示させる関数
void enter_bango(void){
        FILE *pLCD;

        char szHello[] = "Enter Number:";


        pLCD = fopen(LCD_NAME, "w");

        if(pLCD){
                fwrite(szHello, strlen(szHello), 1, pLCD);
                fclose(pLCD);
        }else{
                printf("failed lcd");
        }

}

void seg_display(int value){
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
}


void slot(int HEX_num,int value){
	value = value%10;
switch(HEX_num){
	case 0:
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
      	  break;
      case 1:
            switch(value){
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
            break;
            case 2:
                  switch(value){
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
                  break;
                  case 3:
                        switch(value){
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
                        break;

}
}

void bool_check(bool flag7,bool flag11,bool flag13,bool flag14){
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
