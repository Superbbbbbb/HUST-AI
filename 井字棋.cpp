#include<stdio.h>
#include<string.h>
#include<windows.h>

int ttt[3][3],cnt,p,e;

void Print(){
	printf("\n");
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(ttt[i][j]==1) printf("O");
			if(ttt[i][j]==0) printf("-");
			if(ttt[i][j]==-1) printf("X");
		}
		printf("\n");
	}
}

int Win(){
	if((ttt[0][0]==1&&ttt[1][1]==1&&ttt[2][2]==1)||(ttt[2][0]==1&&ttt[1][1]==1&&ttt[0][2]==1)) return 1;
    if((ttt[0][0]==-1&&ttt[1][1]==-1&&ttt[2][2]==-1)||(ttt[2][0]==-1&&ttt[1][1]==-1&&ttt[0][2]==-1)) return -1;
	for(int i=0;i<3;i++){
		if((ttt[i][0]==1&&ttt[i][1]==1&&ttt[i][2]==1)||(ttt[0][i]==1&&ttt[1][i]==1&&ttt[2][i]==1)) return 1;
		if((ttt[i][0]==-1&&ttt[i][1]==-1&&ttt[i][2]==-1)||(ttt[0][i]==-1&&ttt[1][i]==-1&&ttt[2][i]==-1)) return -1;
	}
	return 0;
}

int Back_up(){
	int i,j,tmp[3][3];
	p=e=0;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(!ttt[i][j]) tmp[i][j]=1;
			else tmp[i][j]=ttt[i][j];
		}
	}
	for(i=0;i<3;i++){
		e+=(tmp[i][0]+tmp[i][1]+tmp[i][2])/3;
		e+=(tmp[0][i]+tmp[1][i]+tmp[2][i])/3;
	}
	e+=(tmp[0][0]+tmp[1][1]+tmp[2][2])/3;
	e+=(tmp[2][0]+tmp[1][1]+tmp[0][2])/3;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(!ttt[i][j]) tmp[i][j]=-1;
			else tmp[i][j]=ttt[i][j];
		}
	}
	for(i=0;i<3;i++){
		p+=(tmp[i][0]+tmp[i][1]+tmp[i][2])/3;
		p+=(tmp[0][i]+tmp[1][i]+tmp[2][i])/3;
	}
	p+=(tmp[0][0]+tmp[1][1]+tmp[2][2])/3;
	p+=(tmp[2][0]+tmp[1][1]+tmp[0][2])/3;
	return e+p;
}

int AlphaBeta(int &value,int d,bool first){
	bool prune=0;
	int ab,tmp;
    if(d==6||d+cnt==9)      //搜索结束
        return Back_up(); //返回电脑的胜率
    if(Win()==1){
        value=10;
        return 0;
    }
    if(first) ab=10;
    else ab=-10;
    for(int i=0;i<3&&!prune;i++){
        for(int j=0;j<3&&!prune;j++){
            if(!ttt[i][j]){
                if(first){
                    ttt[i][j]=-1;
                    if(Win()==-1) tmp=-10;
                    else tmp=AlphaBeta(ab,d+1,!first);   //搜索最小倒推值
                    if(tmp<ab) ab=tmp;      //更新倒推值
                    if(ab<value) prune=1;   //β<α,α剪枝
                }
                else{
                    ttt[i][j]=1;
                    if(Win()==1) tmp=10;
                    else tmp=AlphaBeta(ab,d+1,!first);   //搜索最大倒推值
                    if(tmp>ab) ab=tmp;
                    if(ab>value) prune=1;   //α<β,β剪枝
                }
                ttt[i][j]=0;    //复原棋盘
            }
        }
    }
    if(first){
        if(ab>value)
            value=ab; //更新评估值
    }
    else{
        if(ab<value)
            value=ab;
    }
    return ab;
}

void Input(){
	int row,col;
	while(1){
		printf("\nPlayer:");
		scanf("%d%d",&row,&col);
		if(row>0&&row<4&&col>0&&col<4&&ttt[row-1][col-1]==0){
			ttt[row-1][col-1]=-1;
			break;
		}
		else printf("Input Error!\n");
	}
	cnt++;
}

void PvE(){
	int row,col,op,value=-10,tmp=-10,d=1;
	cnt=0;
	memset(ttt,0,sizeof(ttt));
	printf("Board:\n");
	Print();
	while(1){
		printf("\nWho take the first step:\n1.Player.  2.Computer.[ ]\b\b");
		scanf("%d",&op);
		if(op!=1&&op!=2){
		    printf("Input Error!\n");
		    getchar();
		}
		else break;
	}
	if(op==1){
		while(1){
			Input();
			Print();
			if(Win()==-1){
				printf("\nYou win!\n");
				return;
			}
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(!ttt[i][j]){
						ttt[i][j]=1;
						AlphaBeta(tmp,d,1);
						if(Win()==1){
							printf("\nComputer:%d %d\n",i+1,j+1);
							Print();
							printf("\nComputer win!\n");
							return;
						}
						if(tmp>value){
							value=tmp;
							row=i;col=j;
						}
						ttt[i][j]=0;
						tmp=-10;
					}
				}
			}
			if(ttt[row][col]){
                printf("\nYou win!\n");
				return;
			}
			ttt[row][col]=1;
			tmp=value=-10;
			d=1;
	        printf("\nComputer:%d %d\n",row+1,col+1);
			Print();
			cnt++;
			Back_up();
			if(!p){
				printf("\nTie!\n");
				return;
			}
		}
	}
	else{
		while(1){
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(!ttt[i][j]){
						ttt[i][j]=1;
						AlphaBeta(tmp,d,1);
						if(Win()==1){
							printf("\nComputer:%d %d\n",i+1,j+1);
							Print();
							printf("\nComputer win!\n");
							return;
						}
                        if(tmp>value){
							value=tmp;
							row=i;
							col=j;
						}
						ttt[i][j]=0;
						tmp=-10;
					}
				}
			}
			ttt[row][col]=1;
	        printf("\nComputer:%d %d\n",row+1,col+1);
			Print();
			cnt++;
			Input();
			Print();
			Back_up();
			if(!e){
				printf("\nTie!\n");
				return;
			}
			if(Win()==-1){
				printf("\nYou win!\n");
				return;
			}
            tmp=value=-10;
			d=1;
		}
	}
}

int main(){
	int op;
	while(1){
		PvE();
		printf("\n1.Try Again\t0.Exit [ ]\b\b");
		scanf("%d",&op);
		if(!op) break;
		if(op!=1) getchar();
		system("cls");
	}
	return 0;
}
