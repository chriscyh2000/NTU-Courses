#ifndef _PROB45_GENERATOR
#define _PROB45_GENERATOR
#include<stdio.h>
#include<string.h>
typedef unsigned long long uint64_t;
static uint64_t _rnd_rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}
uint64_t _rnd_s[4];
void _rnd_seed(uint64_t _s[4]){_rnd_s[0]=_s[0];_rnd_s[1]=_s[1];_rnd_s[2]=_s[2];_rnd_s[3]=_s[3];}
uint64_t _rnd_next(){
    const uint64_t result=_rnd_rotl(_rnd_s[0]+_rnd_s[3],23)+_rnd_s[0];
    const uint64_t t=_rnd_s[1]<<17;
    _rnd_s[2]^=_rnd_s[0]; _rnd_s[3]^=_rnd_s[1]; _rnd_s[1]^=_rnd_s[2]; _rnd_s[0]^=_rnd_s[3]; _rnd_s[2]^=t;
    _rnd_s[3] = _rnd_rotl(_rnd_s[3], 45);
    return result;
}
int _rnd_randint(int l,int r){
    return (_rnd_next()&(r-l))+l;    
}
void _rnd_genarray(int n,int *a,int l,int r,unsigned char *v,int flush){
    if(v){
        if(flush) memset(v+l,0,(r-l+1)*sizeof(unsigned char));
        for(int i=0;i<n;i++){
            do{
                a[i]=_rnd_randint(l,r);
            }while(v[a[i]]);
            v[a[i]]=1;
        }
    }
    else for(int i=0;i<n;i++) a[i]=_rnd_randint(l,r);
}
int _generator_n,_generator_t,_generator_task,_generator_v;
void _generator_init(){
    scanf("%d%d%d%d",&_generator_task,&_generator_t,&_generator_n,&_generator_v);
    scanf("%llu%llu%llu%llu",_rnd_s,_rnd_s+1,_rnd_s+2,_rnd_s+3);
}
int _generator_gett(){
    return _generator_t;
}
void _generator_getdata(int* rn,int** rp,int **rq,int **rr){
    static int p[10000005],q[10000005],r[10000005];
    static unsigned char _vis[16777216],*vis=_vis+8388608;
    int n=_generator_n;
    if(_generator_task==2||_generator_task==3) _rnd_genarray(n,p,-_generator_v,_generator_v-1,vis,1);
    else  _rnd_genarray(n,p,-_generator_v,_generator_v-1,NULL,0);
    if(_generator_task==2){
        _rnd_genarray(n,q,-_generator_v,_generator_v-1,vis,1);
        _rnd_genarray(n,r,-_generator_v,_generator_v-1,vis,0);
    }else{
        _rnd_genarray(n,q,-_generator_v,_generator_v-1,NULL,0);
        _rnd_genarray(n,r,-_generator_v,_generator_v-1,NULL,0);
    }
    *rn=n; *rp=p; *rq=q; *rr=r;
}
struct{
    void (*init)();
    int (*getT)();
    void (*getData)(int*,int**,int**,int**);
}generator={_generator_init,_generator_gett,_generator_getdata};
#endif