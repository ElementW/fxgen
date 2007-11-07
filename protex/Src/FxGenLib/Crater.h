/* crater.c: Add craters to a heightfield array 
 * by Heiko Eissfeldt
 * heiko@colossus.escape.de
 */

double dissolve(double nsq_rad);

void distribute_craters(float *a, unsigned how_much, int meshwidth, int meshheight, 
      bool wrap, double ch_scale);

float ran1();   /* random number routine */
void seed_ran1(int seed);
