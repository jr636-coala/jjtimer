#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include <time.h>

SDL_Window* _window;
SDL_Renderer* _renderer;
SDL_Event e;

struct{float prev,best,ao3,ao5,ao12,ao100;std::vector<int>times;}times;
struct NumberParts{char thousandths,hundredths,tenths,seconds,decaseconds,minutes,decaminutes,hours;};

struct{
	bool space;
}keys;

bool quit;
int jtime;
float printTime;
bool timing;
long int dateS;

NumberParts getNumberParts(float number){NumberParts o;short f=(number-(int)number)*1000;o.thousandths=f%10;o.hundredths=(f/10)%10;o.tenths=(f/100)%10;o.seconds=(int)number%10;o.decaseconds=((int)number%60)/10;o.minutes=((int)number/60)%10;o.decaminutes=((int)number/600)%6;o.hours=(int)number/3600;return o;};void writeStringToFile(const char*filePath,const char*content,int length){FILE*f=fopen(filePath,"ab");fseek(f,0,SEEK_END);fwrite(content,1,length,f);fclose(f);};char*loadFileToString(const char*filePath){FILE*f=fopen(filePath,"rb");fseek(f,0,SEEK_END);int l=ftell(f);fseek(f,0,SEEK_SET);l-=ftell(f);char*out=new char[l];fread(out,1,l,f);fclose(f);return out;};char* createSaveString(int jtime, int* length) {NumberParts n=getNumberParts(jtime/1000.0f);long int t=dateS;char T=0;while(t){T++;t/=10;}t=jtime;char J=0;while(t){J++;t/=10;}t=n.hours;char H=0;while(t){H++;t/=10;}*length=T+J+(H?:1)+13;char*o=new char[*length];t=T;for(;t--;){o[t]=(dateS%10)+48;dateS/=10;}o[T]=35;t=J;for(;t--;){o[t+T+1]=(jtime%10)+48;jtime/=10;}o[T+J+1]=35;if(H){t=H;for(;t--;){o[t+T+J+2]=(n.hours%10)+48;n.hours/=10;}}else {o[T+J+2]=48;H=1;}t=T+J+H+2;o[t++]=58;o[t++]=n.decaminutes+48;o[t++]=n.minutes+48;o[t++]=58;o[t++]=n.decaseconds+48;o[t++]=n.seconds+48;o[t++]=58;o[t++]=n.tenths+48;o[t++]=n.hundredths+48;o[t++]=n.thousandths+48;o[t++]=10;return o;};

char saveTime(int jtime) {
times.prev=jtime/1000.0f;times.times.push_back(jtime);int length;
char*saveString=createSaveString(jtime,&length);writeStringToFile("times", saveString, length);


	for (int i = 0 ; i < times.times.size(); i++) {
		NumberParts n = getNumberParts(times.times[i]/1000.0f);
		printf("%d#%d#%d:%d%d:%d%d.%d%d%d\n", (int)dateS, times.times[i],n.hours,n.decaminutes,n.minutes,n.decaseconds,n.seconds,n.tenths,n.hundredths,n.thousandths);
	}


};

void drawNumber(char number,char scale,short x,short y){static unsigned char n[]={0xEE,0x24,0xBA,0xB6,0x74,0xD6,0xDE,0xA4,0xFE,0xF4};SDL_Rect r={scale+x,y,7*scale,scale};n[number]&128?SDL_RenderFillRect(_renderer,&r):0;r={x,scale+y,scale,7*scale};n[number]&64?SDL_RenderFillRect(_renderer,&r):0;r={8*scale+x,scale+y,scale,7*scale};n[number]&32?SDL_RenderFillRect(_renderer,&r):0;r={scale+x,8*scale+y,7*scale, scale};n[number]&16?SDL_RenderFillRect(_renderer,&r):0;r={x,9*scale+y,scale,7*scale};n[number]&8?SDL_RenderFillRect(_renderer,&r):0;r={8*scale+x,9*scale+y,scale,7*scale};n[number]&4?SDL_RenderFillRect(_renderer,&r):0;r={scale+x,16*scale+y,7*scale,scale};n[number]&2?SDL_RenderFillRect(_renderer,&r):0;};void drawTimer(){NumberParts n=getNumberParts(printTime);SDL_Rect r;if(n.hours){drawNumber(n.hours,5,270,270);r={330,300,10,10};SDL_RenderFillRect(_renderer,&r);r={330,330,10,10};SDL_RenderFillRect(_renderer,&r);}if(n.minutes|n.decaminutes){drawNumber(n.decaminutes,5,350,270);drawNumber(n.minutes,5,420,270);r={470,300,10,10};SDL_RenderFillRect(_renderer,&r);r={470,330,10,10};SDL_RenderFillRect(_renderer,&r);}drawNumber(n.decaseconds,5,490,270);drawNumber(n.seconds,5,560,270);r={620,345,10,10};SDL_RenderFillRect(_renderer,&r);drawNumber(n.tenths,5,640,270);drawNumber(n.hundredths,5,710,270);drawNumber(n.thousandths,5,780,270);};

void draw() {
	SDL_SetRenderDrawColor(_renderer,0,0,0,255);SDL_RenderClear(_renderer);
	SDL_SetRenderDrawColor(_renderer,255,255,255,255);drawTimer();

//	if(!timing) {
//		drawNumber();
//	}

	SDL_RenderPresent(_renderer);
};


void update(){
static int timeStarted=0;timing?jtime=SDL_GetTicks()-timeStarted:0;if(keys.space){timing=!timing;timing?jtime=0:saveTime(jtime);dateS=time(NULL);timeStarted=SDL_GetTicks();};printTime=jtime/1000.0f;


};

void processInput() {
	keys = {};
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				quit = true;
			break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
					break;
					case SDLK_SPACE:
						keys.space = true;
					break;

					case SDLK_q:
						jtime += 270000;
						printf("%f\n", jtime/1000.0f);
					break;
					case SDLK_e:
						jtime -= 270000;
						printf("%f\n", jtime/1000.0f);
					break;
				}
			break;
			case SDL_MOUSEBUTTONDOWN:

			break;
		}
	}
};

void init(){SDL_Init(SDL_INIT_EVERYTHING);_window=SDL_CreateWindow("JJ Timer",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1280,720,0);_renderer=SDL_CreateRenderer(_window,-1,SDL_RENDERER_ACCELERATED);quit=timing=0;keys={};times={};};
void mainLoop(){while(!quit){processInput();update();draw();}};int main(int argc,char**argv){init();mainLoop();return 0;};
