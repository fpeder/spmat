#include "rl.h"

#define OFF 64
#define RL_MAX_ROW 10000

#define MID(s, e) ((e-s) >> 1)
#define MASK64(s,e) ((0xFFFFFFFFFFFFFFFF << (63-(e)+(s))) >> (s));

static int off=0;
static int id=0;
static int rl=0;
static int16_t ss[RL_MAX_ROW];
static int16_t se[RL_MAX_ROW];

static void split(uint64_t v, int s, int e);
static void merge(int r, uint32_t *pt);


static 
void split(uint64_t v, int s, int e)
{
     uint64_t mask = MASK64(s, e);
     uint64_t tmp = v & mask;
     if (!tmp) return;
     if (tmp == mask) {
          ss[id]=s+off; se[id]=e+off; id++;
          return;
     }
     else {
          split(v, s, s + MID(s, e));
          split(v, s + MID(s, e) + 1, e); 
     }
}


static 
void merge(int r, uint32_t *pt)
{
     for (int i=0; se[i]!=-1; i++) {
          pt[rl++] = POINT_PACK(r, ss[i]);
          while(ss[i+1] == se[i]+1) i++;
          pt[rl++] = POINT_PACK(r, se[i]);
     }
}


void spmat_rl(spmat src, uint32_t *pt)
{
     uint64_t ppix;
     id=0; rl=0; off=0;
     for (int i=0; i < src.pix.M; i++) {
          for (int j=0; j < src.pix.N; j++) {
               ppix = src.pix.data[i*src.pix.N + j];
               split(ppix, 0, 63);
               off += OFF;
          }
          ss[id]=-1; se[id]=-1; id=0; off=0;
          merge(i, pt);
     }
     pt[rl]=0;
}
