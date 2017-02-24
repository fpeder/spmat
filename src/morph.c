#include "morph.h"

#include <stdio.h>


#define PPIX(m, i, k)  m.pix.data[(i)*m.pix.N + k]


#define EROD3_START(in, i, j)                                    \
     PPIX(in, i, j)  &                                           \
     (PPIX(in, i, j) >> 1) &                                     \
     ((PPIX(in, i, j) << 1) | (PPIX(in, i, j+1) >> 63))

#define EROD3_STEP(in, i, j)                                   \
     PPIX(in, i, j) &                                          \
     ((PPIX(in, i, j) << 1) | (PPIX(in, i, j+1) >> 63)) &      \
     ((PPIX(in, i, j) >> 1) | (PPIX(in, i, j-1) << 63))        \
     
#define EROD3_STOP(in, i, j)                                   \
     PPIX(in, i, j) &                                          \
     (PPIX(in, i, j) << 1) &                                   \
     ((PPIX(in, i, j) >> 1) | (PPIX(in, i, j-1) << 63))

#define EROD5_START(in, i, j)                                   \
     EROD3_START(in, i, j) &                                    \
     (PPIX(in, i, j) >> 2) &                                   \
     ((PPIX(in, i, j) << 2) | (PPIX(in, i, j+1) >> 62)) 

#define EROD5_STEP(in, i, j)                                    \
     EROD3_STEP(in, i, j) &                                     \
     ((PPIX(in, i, j) << 2) | (PPIX(in, i, j+1) >> 62)) &     \
     ((PPIX(in, i, j) >> 2) | (PPIX(in, i, j-1) << 62)) 

#define EROD5_STOP(in, i, j)                                         \
     EROD3_STOP(in, i, j) &                                          \
     (PPIX(in, i, j) << 2) &                                        \
     ((PPIX(in, i, j) >> 2) | (PPIX(in, i, j-1) << 62)) 

#define EROD7_START(in, i, j)                                   \
     EROD5_START(in, i, j) &                                    \
     (PPIX(in, i, j) >> 3) &                                   \
     ((PPIX(in, i, j) << 3) | (PPIX(in, i, j+1) >> 61))

#define EROD7_STEP(in, i, j) \
     EROD5_STEP(in, i, j) & \
     ((PPIX(in, i, j) << 3) | (PPIX(in, i, j+1) >> 61)) &     \
     ((PPIX(in, i, j) >> 3) | (PPIX(in, i, j-1) << 61))

#define EROD7_STOP(in, i, j) \
     EROD5_STOP(in, i, j) & \
     (PPIX(in, i, j) << 3) &                                  \
     ((PPIX(in, i, j) >> 3) | (PPIX(in, i, j-1) << 61))


static void spmat_erode3(spmat in, spmat ou);
static void spmat_erode5(spmat in, spmat ou);
static void spmat_erode7(spmat in, spmat ou);


void spmat_erode(spmat in, spmat ou, int k)
{
     switch (k) {
     case 3: spmat_erode3(in, ou); break;
     case 5: spmat_erode5(in, ou); break;
     case 7: spmat_erode7(in, ou); break;
     default:
          fprintf(stderr, "err: nknown kernel, skip\n");
     }
}


static 
void spmat_erode3(spmat in, spmat ou)
{
     for (int i=1, j=0; i < in.pix.M - 1; i++) {

          PPIX(ou, i, j) = (EROD3_START(in, i-1, j) &
                            EROD3_START(in, i  , j) &
                            EROD3_START(in, i+1, j));

          for (j = 1; j < in.pix.N - 1; j++) {
               PPIX(ou, i, j) = (EROD3_STEP(in, i-1, j) &
                                 EROD3_STEP(in, i  , j) &
                                 EROD3_STEP(in, i+1, j));
          }

          PPIX(ou, i, j) = (EROD3_STOP(in, i-1, j) &
                            EROD3_STOP(in, i  , j) &
                            EROD3_STOP(in, i+1, j));
     }
}


static
void spmat_erode5(spmat in, spmat ou)
{
     for (int i=2, j=0; i < in.pix.M - 2; i++) {
           PPIX(ou, i, j) = (EROD5_START(in, i-2, j) &
                             EROD5_START(in, i-1, j) &
                             EROD5_START(in, i,   j) &
                             EROD5_START(in, i+1, j) &
                             EROD5_START(in, i+2, j));

          for (j=1; j < in.pix.N - 1; j++) {
               PPIX(ou, i, j) = (EROD5_STEP(in, i-2, j) &
                                 EROD5_STEP(in, i-1, j) &
                                 EROD5_STEP(in, i  , j) &
                                 EROD5_STEP(in, i+1, j) &
                                 EROD5_STEP(in, i+2, j));
          }

          PPIX(ou, i, j) = (EROD5_STEP(in, i-2, j) &
                            EROD5_STEP(in, i-1, j) &
                            EROD5_STEP(in, i  , j) &
                            EROD5_STEP(in, i+1, j) &
                            EROD5_STEP(in, i+2, j));
     }
}


static
void spmat_erode7(spmat in, spmat ou)
{
     for (int i=3, j=0; i < in.pix.M - 3; i++) {
          PPIX(ou, i, j) = (EROD7_START(in, i-3, j) &
                             EROD7_START(in, i-2, j) &
                             EROD7_START(in, i-1, j) &
                             EROD7_START(in, i  , j) &
                             EROD7_START(in, i+1, j) &
                             EROD7_START(in, i+2, j) &
                             EROD7_START(in, i+3, j));

          for (j=1; j < in.pix.N - 1; j++) {
               PPIX(ou, i, j) = (EROD7_STEP(in, i-3, j) &
                                 EROD7_STEP(in, i-2, j) &
                                 EROD7_STEP(in, i-1, j) &
                                 EROD7_STEP(in, i  , j) &
                                 EROD7_STEP(in, i+1, j) &
                                 EROD7_STEP(in, i+2, j) &
                                 EROD7_STEP(in, i+3, j));
          }

          PPIX(ou, i, j) = (EROD7_STOP(in, i-3, j) &
                            EROD7_STOP(in, i-2, j) &
                            EROD7_STOP(in, i-1, j) &
                            EROD7_STOP(in, i  , j) &
                            EROD7_STOP(in, i+1, j) &
                            EROD7_STOP(in, i+2, j) &
                            EROD7_STOP(in, i+3, j));
     }
}


#define DIL3_START(in, i, j)                                    \
     PPIX(in, i, j) |                                          \
     (PPIX(in, i, j) >> 1) |                                   \
     ((PPIX(in, i, j) << 1) | (PPIX(in, i, j+1) >> 63))

#define DIL3_STEP(in, i, j)                                     \
     PPIX(in, i, j) |                                          \
     ((PPIX(in, i, j) << 1) | (PPIX(in, i, j+1) >> 63)) |     \
     ((PPIX(in, i, j) >> 1) | (PPIX(in, i, j-1) << 63))

#define DIL3_STOP(in, i, j)                                     \
     PPIX(in, i, j) |                                          \
     (PPIX(in, i, j) << 1) |                                   \
     ((PPIX(in, i, j) >> 1) | (PPIX(in, i, j-1) << 63))

#define DIL5_START(in, i, j)                            \
     DIL3_START(in, i, j) |                             \
     (PPIX(in, i, j) >> 2) |                           \
     ((PPIX(in, i, j) << 2) | (PPIX(in, i, j) >> 62))

#define DIL5_STEP(in, i, j)                               \
     DIL3_STEP(in, i, j) |                                \
     ((PPIX(in, i, j) << 2) | (PPIX(in, i, j) >> 62)) | \
     ((PPIX(in, i, j) >> 2) | (PPIX(in, i, j) << 62))

#define DIL5_STOP(in, i, j)                                   \
     DIL3_STOP(in, i, j) |                                    \
     (PPIX(in, i, j) << 2) |                                 \
     ((PPIX(in, i, j) >> 2) | (PPIX(in, i, j) << 62))

#define DIL7_START(in, i, j)                                    \
     DIL5_START(in, i, j) |                                     \
     (PPIX(in, i, j) >> 3) |                                   \
     ((PPIX(in, i, j) << 3) | (PPIX(in, i, j+1) >> 61))

#define DIL7_STEP(in, i, j)                                     \
     DIL5_STEP(in, i, j) |                                      \
     ((PPIX(in, i, j) << 3) | (PPIX(in, i, j+1) >> 61)) &     \
     ((PPIX(in, i, j) >> 3) | (PPIX(in, i, j-1) << 61))

#define DIL7_STOP(in, i, j)                                    \
     DIL5_STOP(in, i, j) |                                     \
     (PPIX(in, i, j) << 3) |                                  \
     ((PPIX(in, i, j) >> 3) | (PPIX(in, i, j-1) << 61))


static void spmat_dilate3(spmat in, spmat ou);
static void spmat_dilate5(spmat in, spmat ou);
static void spmat_dilate7(spmat in, spmat out);


void spmat_dilate(spmat in, spmat ou, int k)
{
     switch (k) {
     case 3: spmat_dilate3(in, ou); break;
     case 5: spmat_dilate5(in, ou); break;
     case 7: spmat_dilate7(in, ou); break;
     default:
          fprintf(stderr, "err: nknown kernel, skip\n");
     }
}


void spmat_dilate3(spmat in, spmat ou)
{
     for (int i=1, j=0; i < in.pix.M - 1; i++) {

          PPIX(ou, i, j) = (DIL3_START(in, i-1, j) |
                            DIL3_START(in, i  , j) |
                            DIL3_START(in, i+1, j));

          for (j = 1; j < in.pix.N - 1; j++) {
               PPIX(ou, i, j) = (DIL3_STEP(in, i-1, j) |
                                 DIL3_STEP(in, i  , j) |
                                 DIL3_STEP(in, i+1, j));
          }

          PPIX(ou, i, j) = (DIL3_STOP(in, i-1, j) |
                            DIL3_STOP(in, i  , j) |
                            DIL3_STOP(in, i+1, j));
     }
}


void spmat_dilate5(spmat in, spmat ou)
{
     for (int i=2, j=0; i < in.pix.M - 2; i++) {

          PPIX(ou, i, j) = (DIL5_START(in, i-2, j) |
                            DIL5_START(in, i-1, j) |
                            DIL5_START(in, i  , j) |
                            DIL5_START(in, i+1, j) |
                            DIL5_START(in, i+2, j));

          for (j = 1; j < in.pix.N - 1; j++) {
               PPIX(ou, i, j) = (DIL5_STEP(in, i-2, j) |
                                  DIL5_STEP(in, i-1, j) |
                                  DIL5_STEP(in, i  , j) |
                                  DIL5_STEP(in, i+1, j) |
                                  DIL5_STEP(in, i+2, j));
          }

          PPIX(ou, i, j) = (DIL5_STOP(in, i-2, j) |
                            DIL5_STOP(in, i-1, j) |
                            DIL5_STOP(in, i  , j) |
                            DIL5_STOP(in, i+1, j) |
                            DIL5_STOP(in, i+2, j));
     }
}


void spmat_dilate7(spmat in, spmat ou)
{
     for (int i=3, j=0; i < in.pix.M - 3; i++) {

          PPIX(ou, i, j) = (DIL7_START(in, i-3, j) |
                            DIL7_START(in, i-2, j) |
                            DIL7_START(in, i-1, j) |
                            DIL7_START(in, i  , j) |
                            DIL7_START(in, i+1, j) |
                            DIL7_START(in, i+2, j) |
                            DIL7_START(in, i+3, j));
          
          for (j = 1; j < in.pix.N - 1; j++) {
               PPIX(ou, i, j) = (DIL7_STEP(in, i-3, j) |
                                 DIL7_STEP(in, i-2, j) |
                                 DIL7_STEP(in, i-1, j) |
                                 DIL7_STEP(in, i  , j) |
                                 DIL7_STEP(in, i+1, j) |
                                 DIL7_STEP(in, i+2, j) |
                                 DIL7_STEP(in, i+3, j));
          }

          PPIX(ou, i, j) = (DIL7_STOP(in, i-3, j) | 
                            DIL7_STOP(in, i-2, j) |
                            DIL7_STOP(in, i-1, j) |
                            DIL7_STOP(in, i  , j) |
                            DIL7_STOP(in, i+1, j) |
                            DIL7_STOP(in, i+2, j) |
                            DIL7_STOP(in, i+3, j));
     }
}
