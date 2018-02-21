/**
 * Copyright (c) 2017 Armando Faz <armfazh@ic.unicamp.br>.
 * Institute of Computing.
 * University of Campinas, Brazil.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as   
 * published by the Free Software Foundation, version 2 or greater.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "fp25519_x64.h"

/**
 *
 * @param c Two 512-bit products: c[0:7]=a[0:3]*b[0:3] and c[8:15]=a[4:7]*b[4:7]
 * @param a Two 256-bit integers: a[0:3] and a[4:7]
 * @param b Two 256-bit integers: b[0:3] and b[4:7]
 */
void mul2_256x256_integer_x64(uint64_t *const c, uint64_t *const a,
                              uint64_t *const b) {
#ifdef __BMI2__
#ifdef __ADX__
  __asm__ __volatile__(
    "movq   (%1), %%rdx ;" /* A[0] */
    "mulx   (%2),  %%r8,  %%r9 ;" /* A[0]*B[0] */    "xorl %%r10d, %%r10d ;"                           "movq  %%r8,  (%0) ;"
    "mulx  8(%2), %%r10, %%r11 ;" /* A[0]*B[1] */    "adox  %%r9, %%r10 ;"                             "movq %%r10, 8(%0) ;"
    "mulx 16(%2), %%r12, %%r13 ;" /* A[0]*B[2] */    "adox %%r11, %%r12 ;"
    "mulx 24(%2), %%r14, %%rdx ;" /* A[0]*B[3] */    "adox %%r13, %%r14 ;"                                                       "movq    $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"

    "movq  8(%1), %%rdx ;" /* A[1] */
    "mulx   (%2),  %%r8,  %%r9 ;" /* A[1]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 8(%0),  %%r8 ;"    "movq  %%r8,  8(%0) ;"
    "mulx  8(%2), %%r10, %%r11 ;" /* A[1]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 16(%0) ;"
    "mulx 16(%2), %%r12, %%r13 ;" /* A[1]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"                              "movq $0, %%r8  ;"
    "mulx 24(%2), %%r14, %%rdx ;" /* A[1]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"                              "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"

    "movq 16(%1), %%rdx ;" /* A[2] */
    "mulx   (%2),  %%r8,  %%r9 ;" /* A[2]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 16(%0), %%r8 ;"    "movq  %%r8, 16(%0) ;"
    "mulx  8(%2), %%r10, %%r11 ;" /* A[2]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 24(%0) ;"
    "mulx 16(%2), %%r12, %%r13 ;" /* A[2]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"                              "movq $0, %%r8  ;"
    "mulx 24(%2), %%r14, %%rdx ;" /* A[2]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"                              "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"

    "movq 24(%1), %%rdx ;" /* A[3] */
    "mulx   (%2),  %%r8,  %%r9 ;" /* A[3]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 24(%0), %%r8 ;"    "movq  %%r8, 24(%0) ;"
    "mulx  8(%2), %%r10, %%r11 ;" /* A[3]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 32(%0) ;"
    "mulx 16(%2), %%r12, %%r13 ;" /* A[3]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"    "movq %%r12, 40(%0) ;"    "movq $0, %%r8  ;"
    "mulx 24(%2), %%r14, %%rdx ;" /* A[3]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"    "movq %%r14, 48(%0) ;"    "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"    "movq %%rax, 56(%0) ;"

    "movq 32(%1), %%rdx ;" /* A[0] */
    "mulx 32(%2),  %%r8,  %%r9 ;" /* A[0]*B[0] */    "xorl %%r10d, %%r10d ;"                           "movq  %%r8, 64(%0) ;"
    "mulx 40(%2), %%r10, %%r11 ;" /* A[0]*B[1] */    "adox  %%r9, %%r10 ;"                             "movq %%r10, 72(%0) ;"
    "mulx 48(%2), %%r12, %%r13 ;" /* A[0]*B[2] */    "adox %%r11, %%r12 ;"
    "mulx 56(%2), %%r14, %%rdx ;" /* A[0]*B[3] */    "adox %%r13, %%r14 ;"                                                       "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"

    "movq 40(%1), %%rdx ;" /* A[1] */
    "mulx 32(%2),  %%r8,  %%r9 ;" /* A[1]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 72(%0), %%r8 ;"    "movq  %%r8, 72(%0) ;"
    "mulx 40(%2), %%r10, %%r11 ;" /* A[1]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 80(%0) ;"
    "mulx 48(%2), %%r12, %%r13 ;" /* A[1]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"                              "movq $0, %%r8  ;"
    "mulx 56(%2), %%r14, %%rdx ;" /* A[1]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"                              "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"

    "movq 48(%1), %%rdx ;" /* A[2] */
    "mulx 32(%2),  %%r8,  %%r9 ;" /* A[2]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 80(%0), %%r8 ;"    "movq  %%r8, 80(%0) ;"
    "mulx 40(%2), %%r10, %%r11 ;" /* A[2]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 88(%0) ;"
    "mulx 48(%2), %%r12, %%r13 ;" /* A[2]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"                              "movq $0, %%r8  ;"
    "mulx 56(%2), %%r14, %%rdx ;" /* A[2]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"                              "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"

    "movq 56(%1), %%rdx ;" /* A[3] */
    "mulx 32(%2),  %%r8,  %%r9 ;" /* A[3]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 88(%0), %%r8 ;"    "movq  %%r8,  88(%0);"
    "mulx 40(%2), %%r10, %%r11 ;" /* A[3]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10,  96(%0);"
    "mulx 48(%2), %%r12, %%r13 ;" /* A[3]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"    "movq %%r12, 104(%0);"    "movq $0, %%r8  ;"
    "mulx 56(%2), %%r14, %%rdx ;" /* A[3]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"    "movq %%r14, 112(%0);"    "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"    "movq %%rax, 120(%0);"
  :
  : "r"  (c), "r" (a), "r"  (b)
  : "memory", "cc", "%rax", "%rdx",
    "%r8",  "%r9", "%r10", "%r11",
    "%r12", "%r13", "%r14"
  );
#else
  __asm__ __volatile__(
    "movq   (%1), %%rdx; " /* A[0] */
    "mulx   (%2),  %%r8, %%r15; " /* A[0]*B[0] */                           "movq %%r8,  (%0) ;"
    "mulx  8(%2), %%r10, %%rax; " /* A[0]*B[1] */    "addq %%r10, %%r15 ;"
    "mulx 16(%2), %%r12, %%rbx; " /* A[0]*B[2] */    "adcq %%r12, %%rax ;"
    "mulx 24(%2), %%r14, %%rcx; " /* A[0]*B[3] */    "adcq %%r14, %%rbx ;"
    /*******************************************/    "adcq    $0, %%rcx ;"

    "movq  8(%1), %%rdx; " /* A[1] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[1]*B[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 8(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[1]*B[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[1]*B[2] */    "adcq %%r12, %%r11 ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[1]*B[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rax,  %%r9  ;"  "movq %%r9, %%r15 ;"  "movq $0, %%rax ;"
    "adcq %%r11, %%rbx  ;"
    "adcq %%r13, %%rcx  ;"
    "adcq %%rdx, %%rax  ;"

    "movq 16(%1), %%rdx; " /* A[2] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[2]*B[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 16(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[2]*B[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[2]*B[2] */    "adcq %%r12, %%r11 ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[2]*B[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rbx,  %%r9  ;"  "movq %%r9, %%r15 ;"  "movq $0, %%rbx ;"
    "adcq %%r11, %%rcx  ;"
    "adcq %%r13, %%rax  ;"
    "adcq %%rdx, %%rbx  ;"

    "movq 24(%1), %%rdx; " /* A[3] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[3]*B[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 24(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[3]*B[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[3]*B[2] */    "adcq %%r12, %%r11 ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[3]*B[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rcx,  %%r9  ;"  "movq  %%r9, 32(%0) ;"  "movq $0, %%rcx ;"
    "adcq %%r11, %%rax  ;"  "movq %%rax, 40(%0) ;"
    "adcq %%r13, %%rbx  ;"  "movq %%rbx, 48(%0) ;"
    "adcq %%rdx, %%rcx  ;"  "movq %%rcx, 56(%0) ;"

    "movq 32(%1), %%rdx; " /* C[0] */
    "mulx 32(%2),  %%r8, %%r15; " /* C[0]*D[0] */                           "movq %%r8, 64(%0) ;"
    "mulx 40(%2), %%r10, %%rax; " /* C[0]*D[1] */    "addq %%r10, %%r15 ;"
    "mulx 48(%2), %%r12, %%rbx; " /* C[0]*D[2] */    "adcq %%r12, %%rax ;"
    "mulx 56(%2), %%r14, %%rcx; " /* C[0]*D[3] */    "adcq %%r14, %%rbx ;"
    /*******************************************/    "adcq    $0, %%rcx ;"

    "movq 40(%1), %%rdx; " /* C[1] */
    "mulx 32(%2),  %%r8,  %%r9; " /* C[1]*D[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 72(%0) ;"
    "mulx 40(%2), %%r10, %%r11; " /* C[1]*D[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 48(%2), %%r12, %%r13; " /* C[1]*D[2] */    "adcq %%r12, %%r11 ;"
    "mulx 56(%2), %%r14, %%rdx; " /* C[1]*D[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rax,  %%r9  ;"  "movq %%r9, %%r15 ;"  "movq $0, %%rax ;"
    "adcq %%r11, %%rbx  ;"
    "adcq %%r13, %%rcx  ;"
    "adcq %%rdx, %%rax  ;"

    "movq 48(%1), %%rdx; " /* C[2] */
    "mulx 32(%2),  %%r8,  %%r9; " /* C[2]*D[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 80(%0) ;"
    "mulx 40(%2), %%r10, %%r11; " /* C[2]*D[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 48(%2), %%r12, %%r13; " /* C[2]*D[2] */    "adcq %%r12, %%r11 ;"
    "mulx 56(%2), %%r14, %%rdx; " /* C[2]*D[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rbx,  %%r9  ;"  "movq %%r9, %%r15 ;"  "movq $0, %%rbx ;"
    "adcq %%r11, %%rcx  ;"
    "adcq %%r13, %%rax  ;"
    "adcq %%rdx, %%rbx  ;"

    "movq 56(%1), %%rdx; " /* C[3] */
    "mulx 32(%2),  %%r8,  %%r9; " /* C[3]*D[0] */    "addq %%r15,  %%r8 ;"    "movq %%r8, 88(%0) ;"
    "mulx 40(%2), %%r10, %%r11; " /* C[3]*D[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 48(%2), %%r12, %%r13; " /* C[3]*D[2] */    "adcq %%r12, %%r11 ;"
    "mulx 56(%2), %%r14, %%rdx; " /* C[3]*D[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rcx,  %%r9  ;"  "movq  %%r9,  96(%0) ;"  "movq $0, %%rcx ;"
    "adcq %%r11, %%rax  ;"  "movq %%rax, 104(%0) ;"
    "adcq %%r13, %%rbx  ;"  "movq %%rbx, 112(%0) ;"
    "adcq %%rdx, %%rcx  ;"  "movq %%rcx, 120(%0) ;"
  :
  : "r" (c), "r" (a), "r" (b)
  : "memory", "cc", "%rax", "%rbx", "%rcx", "%rdx", "%r8",
    "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"
  );
#endif
#else   /* Without BMI2 */
  /**
  * TODO: Multiplications using MULQ instruction.
  **/
#endif
}

/**
 *
 * @param c
 * @param a
 */
void sqr2_256x256_integer_x64(uint64_t *const c, uint64_t *const a) {
#ifdef __BMI2__
  __asm__ __volatile__(
    "movq  (%1), %%rdx        ; " /* A[0]   */
    "mulx %%rdx,  %%r8, %%r9  ; " /* A[0]^2 */
    "movq 8(%1), %%rdx        ; " /* A[1]   */
    "mulx %%rdx, %%r10, %%r11 ; " /* A[1]^2 */
    "movq  %%r8,   (%0) ;"
    "movq  %%r9,  8(%0) ;"
    "movq %%r10, 16(%0) ;"
    "movq %%r11, 24(%0) ;"

    "movq 16(%1), %%rdx       ; " /* A[2]   */
    "mulx %%rdx,  %%r8, %%r9  ; " /* A[2]^2 */
    "movq 24(%1), %%rdx       ; " /* A[3]   */
    "mulx %%rdx, %%r10, %%r11 ; " /* A[3]^2 */
    "movq  %%r8, 32(%0) ;"
    "movq  %%r9, 40(%0) ;"
    "movq %%r10, 48(%0) ;"
    "movq %%r11, 56(%0) ;"

    "movq  8(%1), %%rdx        ; " /* A[1]      */
    "mulx   (%1), %%r8, %%r9   ; " /* A[0]*A[1] */
    "mulx 16(%1), %%r10, %%r11 ; " /* A[2]*A[1] */
    "mulx 24(%1), %%rcx, %%r14 ; " /* A[3]*A[1] */

    "movq 16(%1), %%rdx        ; " /* A[2]      */
    "mulx 24(%1), %%r12, %%r13 ; " /* A[3]*A[2] */
    "mulx   (%1), %%rax, %%rdx ; " /* A[0]*A[2] */

    "addq %%rax, %%r9  ;"
    "adcq %%rdx, %%r10 ;"
    "adcq %%rcx, %%r11 ;"
    "adcq %%r14, %%r12 ;"
    "adcq    $0, %%r13 ;"
    "movq    $0, %%r14 ;"
    "adcq    $0, %%r14 ;"

    "movq   (%1), %%rdx        ; " /* A[0]      */
    "mulx 24(%1), %%rax, %%rdx ; " /* A[0]*A[3] */

    "addq %%rax, %%r10 ;"
    "adcq %%rdx, %%r11 ;"
    "adcq    $0, %%r12 ;"
    "adcq    $0, %%r13 ;"
    "adcq    $0, %%r14 ;"

    "shldq $1, %%r13, %%r14 ;"
    "shldq $1, %%r12, %%r13 ;"
    "shldq $1, %%r11, %%r12 ;"
    "shldq $1, %%r10, %%r11 ;"
    "shldq $1,  %%r9, %%r10 ;"
    "shldq $1,  %%r8,  %%r9 ;"
    "shlq  $1,  %%r8        ;"

    "addq  8(%0),  %%r8 ;"  "movq  %%r8,  8(%0) ;"
    "adcq 16(%0),  %%r9 ;"  "movq  %%r9, 16(%0) ;"
    "adcq 24(%0), %%r10 ;"  "movq %%r10, 24(%0) ;"
    "adcq 32(%0), %%r11 ;"  "movq %%r11, 32(%0) ;"
    "adcq 40(%0), %%r12 ;"  "movq %%r12, 40(%0) ;"
    "adcq 48(%0), %%r13 ;"  "movq %%r13, 48(%0) ;"
    "adcq 56(%0), %%r14 ;"  "movq %%r14, 56(%0) ;"

    "movq  32(%1), %%rdx      ; " /* A[0]   */
    "mulx %%rdx,  %%r8, %%r9  ; " /* A[0]^2 */
    "movq  40(%1), %%rdx      ; " /* A[1]   */
    "mulx %%rdx, %%r10, %%r11 ; " /* A[1]^2 */
    "movq  %%r8, 64(%0) ;"
    "movq  %%r9, 72(%0) ;"
    "movq %%r10, 80(%0) ;"
    "movq %%r11, 88(%0) ;"

    "movq 48(%1), %%rdx       ; " /* A[2]   */
    "mulx %%rdx,  %%r8, %%r9  ; " /* A[2]^2 */
    "movq 56(%1), %%rdx       ; " /* A[3]   */
    "mulx %%rdx, %%r10, %%r11 ; " /* A[3]^2 */
    "movq  %%r8,  96(%0) ;"
    "movq  %%r9, 104(%0) ;"
    "movq %%r10, 112(%0) ;"
    "movq %%r11, 120(%0) ;"

    "movq 40(%1), %%rdx        ; " /* A[1]      */
    "mulx 32(%1), %%r8, %%r9   ; " /* A[0]*A[1] */
    "mulx 48(%1), %%r10, %%r11 ; " /* A[2]*A[1] */
    "mulx 56(%1), %%rcx, %%r14 ; " /* A[3]*A[1] */

    "movq 48(%1), %%rdx        ; " /* A[2]      */
    "mulx 56(%1), %%r12, %%r13 ; " /* A[3]*A[2] */
    "mulx 32(%1), %%rax, %%rdx ; " /* A[0]*A[2] */

    "addq %%rax,  %%r9 ;"
    "adcq %%rdx, %%r10 ;"
    "adcq %%rcx, %%r11 ;"
    "adcq %%r14, %%r12 ;"
    "adcq    $0, %%r13 ;"
    "movq    $0, %%r14 ;"
    "adcq    $0, %%r14 ;"

    "movq 32(%1), %%rdx        ; " /* A[0]      */
    "mulx 56(%1), %%rax, %%rdx ; " /* A[0]*A[3] */

    "addq %%rax, %%r10 ;"
    "adcq %%rdx, %%r11 ;"
    "adcq    $0, %%r12 ;"
    "adcq    $0, %%r13 ;"
    "adcq    $0, %%r14 ;"

    "shldq $1, %%r13, %%r14 ;"
    "shldq $1, %%r12, %%r13 ;"
    "shldq $1, %%r11, %%r12 ;"
    "shldq $1, %%r10, %%r11 ;"
    "shldq $1,  %%r9, %%r10 ;"
    "shldq $1,  %%r8,  %%r9 ;"
    "shlq  $1,  %%r8        ;"

    "addq  72(%0),  %%r8 ;"  "movq   %%r8,  72(%0) ;"
    "adcq  80(%0),  %%r9 ;"  "movq   %%r9,  80(%0) ;"
    "adcq  88(%0), %%r10 ;"  "movq  %%r10,  88(%0) ;"
    "adcq  96(%0), %%r11 ;"  "movq  %%r11,  96(%0) ;"
    "adcq 104(%0), %%r12 ;"  "movq  %%r12, 104(%0) ;"
    "adcq 112(%0), %%r13 ;"  "movq  %%r13, 112(%0) ;"
    "adcq 120(%0), %%r14 ;"  "movq  %%r14, 120(%0) ;"
  :
  : "r" (c), "r" (a)
  : "memory", "cc", "%rax", "%rcx", "%rdx", "%r8",
    "%r9", "%r10", "%r11", "%r12", "%r13", "%r14"
  );
#else    /* Without BMI2 */
  /**
  * TODO: Multiplications using MULQ instruction.
  **/
#endif
}

/**
 *
 * @param c
 * @param a
 */
void red_EltFp25519_2w_x64(uint64_t *const c, uint64_t *const a) {
#ifdef __BMI2__
#ifdef __ADX__
  __asm__ __volatile__(
    "movl    $38, %%edx; " /* 2*c = 38 = 2^256 */
    "mulx 32(%1),  %%r8, %%r10; " /* c*C[4] */   "xorl %%ebx, %%ebx ;"  "adox   (%1),  %%r8 ;"
    "mulx 40(%1),  %%r9, %%r11; " /* c*C[5] */   "adcx %%r10,  %%r9 ;"  "adox  8(%1),  %%r9 ;"
    "mulx 48(%1), %%r10, %%rax; " /* c*C[6] */   "adcx %%r11, %%r10 ;"  "adox 16(%1), %%r10 ;"
    "mulx 56(%1), %%r11, %%rcx; " /* c*C[7] */   "adcx %%rax, %%r11 ;"  "adox 24(%1), %%r11 ;"
    /****************************************/   "adcx %%rbx, %%rcx ;"  "adox  %%rbx, %%rcx ;"
    "clc ;"
    "mulx %%rcx, %%rax, %%rcx ; " /* c*C[4] */
    "adcx %%rax,  %%r8 ;"
    "adcx %%rcx,  %%r9 ;"  "movq  %%r9,  8(%0) ;"
    "adcx %%rbx, %%r10 ;"  "movq %%r10, 16(%0) ;"
    "adcx %%rbx, %%r11 ;"  "movq %%r11, 24(%0) ;"
    "setc %%al         ;"
    "negq %%rax        ;"
    "andq   $38, %%rax ;"
    "addq %%rax,  %%r8 ;"
    "movq  %%r8,  (%0) ;"

    "mulx  96(%1),  %%r8, %%r10; " /* c*C[4] */  "xorl %%ebx, %%ebx ;"  "adox 64(%1),  %%r8 ;"
    "mulx 104(%1),  %%r9, %%r11; " /* c*C[5] */  "adcx %%r10,  %%r9 ;"  "adox 72(%1),  %%r9 ;"
    "mulx 112(%1), %%r10, %%rax; " /* c*C[6] */  "adcx %%r11, %%r10 ;"  "adox 80(%1), %%r10 ;"
    "mulx 120(%1), %%r11, %%rcx; " /* c*C[7] */  "adcx %%rax, %%r11 ;"  "adox 88(%1), %%r11 ;"
    /*****************************************/  "adcx %%rbx, %%rcx ;"  "adox  %%rbx, %%rcx ;"
    "clc ;"
    "mulx %%rcx, %%rax, %%rcx ; " /* c*C[4] */
    "adcx %%rax,  %%r8 ;"
    "adcx %%rcx,  %%r9 ;"  "movq  %%r9, 40(%0) ;"
    "adcx %%rbx, %%r10 ;"  "movq %%r10, 48(%0) ;"
    "adcx %%rbx, %%r11 ;"  "movq %%r11, 56(%0) ;"
    "setc %%al         ;"
    "negq %%rax        ;"
    "andq   $38, %%rax ;"
    "addq %%rax,  %%r8 ;"
    "movq  %%r8, 32(%0);"
  :
  : "r" (c), "r" (a)
  : "memory", "cc", "%rax", "%rbx", "%rcx", "%rdx", "%r8", "%r9", "%r10", "%r11"
  );
#else
  __asm__ __volatile__(
    "movl    $38, %%edx ; " /* 2*c = 38 = 2^256 */
    "mulx 32(%1),  %%r8, %%r10 ;" /* c*C[4] */
    "mulx 40(%1),  %%r9, %%r11 ;" /* c*C[5] */  "addq %%r10,  %%r9 ;"
    "mulx 48(%1), %%r10, %%rax ;" /* c*C[6] */  "adcq %%r11, %%r10 ;"
    "mulx 56(%1), %%r11, %%rcx ;" /* c*C[7] */  "adcq %%rax, %%r11 ;"
    /****************************************/  "adcq    $0, %%rcx ;"
    "addq   (%1),  %%r8 ;"
    "adcq  8(%1),  %%r9 ;"
    "adcq 16(%1), %%r10 ;"
    "adcq 24(%1), %%r11 ;"
    "adcq     $0, %%rcx ;"
    "mulx %%rcx, %%rax, %%rcx ;" /* c*C[4] */
    "addq %%rax,  %%r8 ;"
    "adcq %%rcx,  %%r9 ;"  "movq  %%r9,  8(%0) ;"
    "adcq    $0, %%r10 ;"  "movq %%r10, 16(%0) ;"
    "adcq    $0, %%r11 ;"  "movq %%r11, 24(%0) ;"
    "setc %%al         ;"
    "negq %%rax        ;"
    "andq   $38, %%rax ;"
    "addq %%rax,  %%r8 ;"
    "movq  %%r8,  (%0) ;"

    "mulx  96(%1),  %%r8, %%r10 ;" /* c*C[4] */
    "mulx 104(%1),  %%r9, %%r11 ;" /* c*C[5] */  "addq %%r10,  %%r9 ;"
    "mulx 112(%1), %%r10, %%rax ;" /* c*C[6] */  "adcq %%r11, %%r10 ;"
    "mulx 120(%1), %%r11, %%rcx ;" /* c*C[7] */  "adcq %%rax, %%r11 ;"
    /*****************************************/  "adcq    $0, %%rcx ;"
    "addq 64(%1),  %%r8 ;"
    "adcq 72(%1),  %%r9 ;"
    "adcq 80(%1), %%r10 ;"
    "adcq 88(%1), %%r11 ;"
    "adcq     $0, %%rcx ;"
    "mulx %%rcx, %%rax, %%rcx ;"  /* c*C[4] */
    "addq %%rax,  %%r8 ;"
    "adcq %%rcx,  %%r9 ;"  "movq  %%r9, 40(%0) ;"
    "adcq    $0, %%r10 ;"  "movq %%r10, 48(%0) ;"
    "adcq    $0, %%r11 ;"  "movq %%r11, 56(%0) ;"
    "setc %%al         ;"
    "negq %%rax        ;"
    "andq   $38, %%rax ;"
    "addq %%rax,  %%r8 ;"
    "movq  %%r8, 32(%0);"
  :
  : "r" (c), "r" (a)
  : "memory", "cc", "%rax", "%rcx", "%rdx", "%r8", "%r9", "%r10", "%r11"
  );
#endif
#else    /* Without BMI2 */
  /* [TODO] */
#endif
}

void mul_256x256_integer_x64(uint64_t *const c, uint64_t *const a, uint64_t *const b) {
#ifdef __BMI2__
#ifdef __ADX__
  __asm__ __volatile__(
    "movq   (%1), %%rdx; " /* A[0] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[0]*B[0] */    "xorl %%r10d, %%r10d ;"                           "movq  %%r8,  (%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[0]*B[1] */    "adox  %%r9, %%r10 ;"                             "movq %%r10, 8(%0) ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[0]*B[2] */    "adox %%r11, %%r12 ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[0]*B[3] */    "adox %%r13, %%r14 ;"                                                       "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"

    "movq  8(%1), %%rdx; " /* A[1] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[1]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 8(%0),  %%r8 ;"    "movq  %%r8,  8(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[1]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 16(%0) ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[1]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"                              "movq $0, %%r8  ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[1]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"                              "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"

    "movq 16(%1), %%rdx; " /* A[2] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[2]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 16(%0), %%r8 ;"    "movq  %%r8, 16(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[2]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 24(%0) ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[2]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"                              "movq $0, %%r8  ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[2]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"                              "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"

    "movq 24(%1), %%rdx; " /* A[3] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[3]*B[0] */    "xorl %%r10d, %%r10d ;"  "adcx 24(%0), %%r8 ;"    "movq  %%r8, 24(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[3]*B[1] */    "adox  %%r9, %%r10 ;"    "adcx %%r12, %%r10 ;"    "movq %%r10, 32(%0) ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[3]*B[2] */    "adox %%r11, %%r12 ;"    "adcx %%r14, %%r12 ;"    "movq %%r12, 40(%0) ;"    "movq $0, %%r8  ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[3]*B[3] */    "adox %%r13, %%r14 ;"    "adcx %%rax, %%r14 ;"    "movq %%r14, 48(%0) ;"    "movq $0, %%rax ;"
    /*******************************************/    "adox %%rdx, %%rax ;"    "adcx  %%r8, %%rax ;"    "movq %%rax, 56(%0) ;"
  :
  : "r" (c), "r" (a), "r" (b)
  : "memory", "cc", "%rax", "%rdx", "%r8",
    "%r9", "%r10", "%r11", "%r12", "%r13", "%r14"
  );
#else
  __asm__ __volatile__(
    "movq   (%1), %%rdx; " /* A[0] */
    "mulx   (%2),  %%r8, %%r15; " /* A[0]*B[0] */                           "movq %%r8,  (%0) ;"
    "mulx  8(%2), %%r10, %%rax; " /* A[0]*B[1] */    "addq %%r10, %%r15 ;"
    "mulx 16(%2), %%r12, %%rbx; " /* A[0]*B[2] */    "adcq %%r12, %%rax ;"
    "mulx 24(%2), %%r14, %%rcx; " /* A[0]*B[3] */    "adcq %%r14, %%rbx ;"
    /*******************************************/    "adcq    $0, %%rcx ;"

    "movq  8(%1), %%rdx; " /* A[1] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[1]*B[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 8(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[1]*B[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[1]*B[2] */    "adcq %%r12, %%r11 ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[1]*B[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rax,  %%r9  ;"  "movq %%r9, %%r15 ;"  "movq $0, %%rax ;"
    "adcq %%r11, %%rbx  ;"
    "adcq %%r13, %%rcx  ;"
    "adcq %%rdx, %%rax  ;"

    "movq 16(%1), %%rdx; " /* A[2] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[2]*B[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 16(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[2]*B[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[2]*B[2] */    "adcq %%r12, %%r11 ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[2]*B[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rbx,  %%r9  ;"  "movq %%r9, %%r15 ;"  "movq $0, %%rbx ;"
    "adcq %%r11, %%rcx  ;"
    "adcq %%r13, %%rax  ;"
    "adcq %%rdx, %%rbx  ;"

    "movq 24(%1), %%rdx; " /* A[3] */
    "mulx   (%2),  %%r8,  %%r9; " /* A[3]*B[0] */    "addq %%r15,  %%r8 ;"  "movq %%r8, 24(%0) ;"
    "mulx  8(%2), %%r10, %%r11; " /* A[3]*B[1] */    "adcq %%r10,  %%r9 ;"
    "mulx 16(%2), %%r12, %%r13; " /* A[3]*B[2] */    "adcq %%r12, %%r11 ;"
    "mulx 24(%2), %%r14, %%rdx; " /* A[3]*B[3] */    "adcq %%r14, %%r13 ;"
    /*******************************************/    "adcq    $0, %%rdx ;"

    "addq %%rcx,  %%r9  ;"  "movq  %%r9, 32(%0) ;"  "movq $0, %%rcx ;"
    "adcq %%r11, %%rax  ;"  "movq %%rax, 40(%0) ;"
    "adcq %%r13, %%rbx  ;"  "movq %%rbx, 48(%0) ;"
    "adcq %%rdx, %%rcx  ;"  "movq %%rcx, 56(%0) ;"
  :
  : "r" (c), "r" (a), "r" (b)
  : "memory", "cc", "%rax", "%rbx", "%rcx", "%rdx", "%r8",
    "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"
  );
#endif
#else    /* Without BMI2 */
  /**
  * TODO: Multiplications using MULQ instruction.
  **/
#endif
}

void sqr_256x256_integer_x64(uint64_t *const c, uint64_t *const a) {
#ifdef __BMI2__
  __asm__ __volatile__(
    "movq  (%1), %%rdx        ; " /* A[0]   */
    "mulx %%rdx,  %%r8, %%r9  ; " /* A[0]^2 */
    "movq 8(%1), %%rdx        ; " /* A[1]   */
    "mulx %%rdx, %%r10, %%r11 ; " /* A[1]^2 */
    "movq  %%r8,   (%0) ;"
    "movq  %%r9,  8(%0) ;"
    "movq %%r10, 16(%0) ;"
    "movq %%r11, 24(%0) ;"

    "movq 16(%1), %%rdx       ; " /* A[2]   */
    "mulx %%rdx,  %%r8, %%r9  ; " /* A[2]^2 */
    "movq 24(%1), %%rdx       ; " /* A[3]   */
    "mulx %%rdx, %%r10, %%r11 ; " /* A[3]^2 */
    "movq  %%r8, 32(%0) ;"
    "movq  %%r9, 40(%0) ;"
    "movq %%r10, 48(%0) ;"
    "movq %%r11, 56(%0) ;"

    "movq  8(%1), %%rdx        ; " /* A[1]      */
    "mulx   (%1), %%r8, %%r9   ; " /* A[0]*A[1] */
    "mulx 16(%1), %%r10, %%r11 ; " /* A[2]*A[1] */
    "mulx 24(%1), %%rcx, %%r14 ; " /* A[3]*A[1] */

    "movq 16(%1), %%rdx        ; " /* A[2]      */
    "mulx 24(%1), %%r12, %%r13 ; " /* A[3]*A[2] */
    "mulx   (%1), %%rax, %%rdx ; " /* A[0]*A[2] */

    "addq %%rax,  %%r9 ;"
    "adcq %%rdx, %%r10 ;"
    "adcq %%rcx, %%r11 ;"
    "adcq %%r14, %%r12 ;"
    "adcq    $0, %%r13 ;"
    "movq    $0, %%r14 ;"
    "adcq    $0, %%r14 ;"

    "movq   (%1), %%rdx        ; " /* A[0]      */
    "mulx 24(%1), %%rax, %%rdx ; " /* A[0]*A[3] */

    "addq %%rax, %%r10 ;"
    "adcq %%rdx, %%r11 ;"
    "adcq    $0, %%r12 ;"
    "adcq    $0, %%r13 ;"
    "adcq    $0, %%r14 ;"

    "shldq $1, %%r13, %%r14 ;"
    "shldq $1, %%r12, %%r13 ;"
    "shldq $1, %%r11, %%r12 ;"
    "shldq $1, %%r10, %%r11 ;"
    "shldq $1,  %%r9, %%r10 ;"
    "shldq $1,  %%r8,  %%r9 ;"
    "shlq  $1,  %%r8        ;"

    "addq  8(%0),  %%r8 ;"  "movq  %%r8,  8(%0) ;"
    "adcq 16(%0),  %%r9 ;"  "movq  %%r9, 16(%0) ;"
    "adcq 24(%0), %%r10 ;"  "movq %%r10, 24(%0) ;"
    "adcq 32(%0), %%r11 ;"  "movq %%r11, 32(%0) ;"
    "adcq 40(%0), %%r12 ;"  "movq %%r12, 40(%0) ;"
    "adcq 48(%0), %%r13 ;"  "movq %%r13, 48(%0) ;"
    "adcq 56(%0), %%r14 ;"  "movq %%r14, 56(%0) ;"
  :
  : "r" (c), "r" (a)
  : "memory", "cc", "%rax", "%rcx", "%rdx", "%r8",
    "%r9", "%r10", "%r11", "%r12", "%r13", "%r14"
  );
#else    /* Without BMI2 */
  /**
  * TODO: Multiplications using MULQ instruction.
  **/
#endif
}

void red_EltFp25519_1w_x64(uint64_t *const c, uint64_t *const a) {
#ifdef __BMI2__
#ifdef __ADX__
  __asm__ __volatile__(
    "movl    $38, %%edx; " /* 2*c = 38 = 2^256 */
    "mulx 32(%1),  %%r8, %%r10; " /* c*C[4] */  "xorl %%ebx, %%ebx ;"  "adox   (%1),  %%r8 ;"
    "mulx 40(%1),  %%r9, %%r11; " /* c*C[5] */  "adcx %%r10,  %%r9 ;"  "adox  8(%1),  %%r9 ;"
    "mulx 48(%1), %%r10, %%rax; " /* c*C[6] */  "adcx %%r11, %%r10 ;"  "adox 16(%1), %%r10 ;"
    "mulx 56(%1), %%r11, %%rcx; " /* c*C[7] */  "adcx %%rax, %%r11 ;"  "adox 24(%1), %%r11 ;"
    /****************************************/  "adcx %%rbx, %%rcx ;"  "adox  %%rbx, %%rcx ;"
    "clc ;"
    "mulx %%rcx, %%rax, %%rcx ; " /* c*C[4] */
    "adcx %%rax,  %%r8 ;"
    "adcx %%rcx,  %%r9 ;"  "movq  %%r9,  8(%0) ;"
    "adcx %%rbx, %%r10 ;"  "movq %%r10, 16(%0) ;"
    "adcx %%rbx, %%r11 ;"  "movq %%r11, 24(%0) ;"
    "setc %%al         ;"
    "negq %%rax        ;"
    "andq   $38, %%rax ;"
    "addq %%rax,  %%r8 ;"  "movq  %%r8,  (%0) ;"
  :
  : "r" (c), "r" (a)
  : "memory", "cc", "%rax", "%rbx", "%rcx", "%rdx", "%r8", "%r9", "%r10", "%r11"
  );
#else
  __asm__ __volatile__(
    "movl    $38, %%edx; " /* 2*c = 38 = 2^256 */
    "mulx 32(%1),  %%r8, %%r10; " /* c*C[4] */
    "mulx 40(%1),  %%r9, %%r11; " /* c*C[5] */  "addq %%r10,  %%r9 ;"
    "mulx 48(%1), %%r10, %%rax; " /* c*C[6] */  "adcq %%r11, %%r10 ;"
    "mulx 56(%1), %%r11, %%rcx; " /* c*C[7] */  "adcq %%rax, %%r11 ;"
    /****************************************/  "adcq    $0, %%rcx ;"
    "addq   (%1),  %%r8 ;"
    "adcq  8(%1),  %%r9 ;"
    "adcq 16(%1), %%r10 ;"
    "adcq 24(%1), %%r11 ;"
    "adcq     $0, %%rcx ;"
    "mulx %%rcx, %%rax, %%rcx ; " /* c*C[4] */
    "addq %%rax,  %%r8 ;"
    "adcq %%rcx,  %%r9 ;"  "movq  %%r9,  8(%0) ;"
    "adcq    $0, %%r10 ;"  "movq %%r10, 16(%0) ;"
    "adcq    $0, %%r11 ;"  "movq %%r11, 24(%0) ;"
    "setc %%al         ;"
    "negq %%rax        ;"
    "andq   $38, %%rax ;"
    "addq %%rax,  %%r8 ;"  "movq  %%r8,  (%0) ;"
  :
  : "r" (c), "r" (a)
  : "memory", "cc", "%rax", "%rcx", "%rdx", "%r8", "%r9", "%r10", "%r11"
  );
#endif
#else    /* Without BMI2 */
  /**
  * TODO: Multiplications using MULQ instruction.
  **/
#endif
}

inline void add_EltFp25519_1w_x64(uint64_t *const c, uint64_t *const a, uint64_t *const b) {
#ifdef __ADX__
  __asm__ __volatile__(
    "clc                ;"
    "movq   (%2),  %%r8 ;"  "adcx   (%1),  %%r8 ;"
    "movq  8(%2),  %%r9 ;"  "adcx  8(%1),  %%r9 ;"
    "movq 16(%2), %%r10 ;"  "adcx 16(%1), %%r10 ;"
    "movq 24(%2), %%r11 ;"  "adcx 24(%1), %%r11 ;"
    "setc %%cl          ;"
    "negq %%rcx         ;"
    "andq   $38, %%rcx  ;"
    "xorl %%eax, %%eax  ;"
    "adcx %%rcx,  %%r8  ;"
    "adcx %%rax,  %%r9  ;"  "movq  %%r9,  8(%0) ;"
    "adcx %%rax, %%r10  ;"  "movq %%r10, 16(%0) ;"
    "adcx %%rax, %%r11  ;"  "movq %%r11, 24(%0) ;"
    "setc %%cl          ;"
    "negq %%rcx         ;"
    "andq   $38, %%rcx  ;"
    "clc                ;"
    "adcx %%rcx,  %%r8  ;"  "movq  %%r8,   (%0) ;"
  :
  : "r" (c), "r" (a), "r" (b)
  : "memory", "cc", "%rax", "%rcx", "%r8", "%r9", "%r10", "%r11"
  );
#else
  __asm__ __volatile__(
    "movq   (%2),  %%r8 ;"  "addq   (%1),  %%r8 ;"
    "movq  8(%2),  %%r9 ;"  "adcq  8(%1),  %%r9 ;"
    "movq 16(%2), %%r10 ;"  "adcq 16(%1), %%r10 ;"
    "movq 24(%2), %%r11 ;"  "adcq 24(%1), %%r11 ;"
    "setc %%cl          ;"
    "negq %%rcx         ;"
    "andq   $38, %%rcx  ;"
    "addq %%rcx,  %%r8  ;"
    "adcq    $0,  %%r9  ;"  "movq  %%r9,  8(%0) ;"
    "adcq    $0, %%r10  ;"  "movq %%r10, 16(%0) ;"
    "adcq    $0, %%r11  ;"  "movq %%r11, 24(%0) ;"
    "setc %%cl          ;"
    "negq %%rcx         ;"
    "andq   $38, %%rcx  ;"
    "addq %%rcx,  %%r8  ;"  "movq  %%r8,   (%0) ;"
  :
  : "r" (c), "r" (a), "r" (b)
  : "memory", "cc", "%rax", "%rcx", "%r8", "%r9", "%r10", "%r11"
  );
#endif
}

inline void sub_EltFp25519_1w_x64(uint64_t *const c, uint64_t *const a, uint64_t *const b) {
  __asm__ __volatile__(
    "movq   (%1),  %%r8 ;"  "subq   (%2),  %%r8 ;"
    "movq  8(%1),  %%r9 ;"  "sbbq  8(%2),  %%r9 ;"
    "movq 16(%1), %%r10 ;"  "sbbq 16(%2), %%r10 ;"
    "movq 24(%1), %%r11 ;"  "sbbq 24(%2), %%r11 ;"
    "setc %%cl          ;"
    "negq %%rcx         ;"
    "andq $38, %%rcx    ;"
    "subq %%rcx,  %%r8  ;"
    "sbbq    $0,  %%r9  ;"  "movq  %%r9,  8(%0) ;"
    "sbbq    $0, %%r10  ;"  "movq %%r10, 16(%0) ;"
    "sbbq    $0, %%r11  ;"  "movq %%r11, 24(%0) ;"
    "setc %%cl          ;"
    "negq %%rcx         ;"
    "andq $38, %%rcx    ;"
    "subq %%rcx,  %%r8  ;"  "movq  %%r8,   (%0) ;"
  :
  : "r" (c), "r" (a), "r" (b)
  : "memory", "cc", "%rax", "%rcx", "%r8", "%r9", "%r10", "%r11"
  );
}

/**
 * Multiplication by a24 = (A+2)/4 = (486662+2)/4 = 121666
 **/
inline void mul_a24_EltFp25519_1w_x64(uint64_t *const c, uint64_t *const a) {
#ifdef __BMI2__
  const uint64_t a24 = 121666;
  __asm__ __volatile__(
    "movq     %2, %%rdx ;"
    "mulx   (%1),  %%r8, %%r10 ;"
    "mulx  8(%1),  %%r9, %%r11 ;"  "addq %%r10,  %%r9 ;"
    "mulx 16(%1), %%r10, %%rax ;"  "adcq %%r11, %%r10 ;"
    "mulx 24(%1), %%r11, %%rcx ;"  "adcq %%rax, %%r11 ;"
    /***************************/  "adcq    $0, %%rcx ;"
    "movl   $38, %%edx; " /* 2*c = 38 = 2^256 mod 2^255-19*/
    "mulx %%rcx, %%rax, %%rcx ;"
    "addq %%rax,  %%r8 ;"
    "adcq %%rcx,  %%r9 ;"  "movq  %%r9,  8(%0) ;"
    "adcq    $0, %%r10 ;"  "movq %%r10, 16(%0) ;"
    "adcq    $0, %%r11 ;"  "movq %%r11, 24(%0) ;"
    "setc  %%al        ;"
    "negq %%rax        ;"
    "andq   $38, %%rax ;"
    "addq %%rax,  %%r8 ;"  "movq  %%r8,  (%0) ;"
  :
  : "r" (c), "r" (a), "r" (a24)
  : "memory", "cc", "%rax", "%rcx", "%rdx", "%r8", "%r9", "%r10", "%r11"
  );
#else /* Without BMI2 */
  /**
  * TODO: Multiplications using MULQ instruction.
  **/
#endif
}

void inv_EltFp25519_1w_x64(uint64_t *const pC, uint64_t *const pA) {
#define sqrn_EltFp25519_1w_x64(a, times)\
  counter = times;\
  while ( counter-- > 0) {\
      sqr_EltFp25519_1w_x64(a);\
  }

  EltFp25519_1w_Buffer_x64 buffer_1w;
  EltFp25519_1w_x64 x0, x1, x2;
  uint64_t * T[5];
  uint64_t counter;

  T[0] = x0;
  T[1] = pC; /* x^(-1) */
  T[2] = x1;
  T[3] = x2;
  T[4] = pA; /* x */

  copy_EltFp25519_1w_x64(T[1], pA);
  sqrn_EltFp25519_1w_x64(T[1], 1);
  copy_EltFp25519_1w_x64(T[2], T[1]);
  sqrn_EltFp25519_1w_x64(T[2], 2);
  mul_EltFp25519_1w_x64(T[0], pA, T[2]);
  mul_EltFp25519_1w_x64(T[1], T[1], T[0]);
  copy_EltFp25519_1w_x64(T[2], T[1]);
  sqrn_EltFp25519_1w_x64(T[2], 1);
  mul_EltFp25519_1w_x64(T[0], T[0], T[2]);
  copy_EltFp25519_1w_x64(T[2], T[0]);
  sqrn_EltFp25519_1w_x64(T[2], 5);
  mul_EltFp25519_1w_x64(T[0], T[0], T[2]);
  copy_EltFp25519_1w_x64(T[2], T[0]);
  sqrn_EltFp25519_1w_x64(T[2], 10);
  mul_EltFp25519_1w_x64(T[2], T[2], T[0]);
  copy_EltFp25519_1w_x64(T[3], T[2]);
  sqrn_EltFp25519_1w_x64(T[3], 20);
  mul_EltFp25519_1w_x64(T[3], T[3], T[2]);
  sqrn_EltFp25519_1w_x64(T[3], 10);
  mul_EltFp25519_1w_x64(T[3], T[3], T[0]);
  copy_EltFp25519_1w_x64(T[0], T[3]);
  sqrn_EltFp25519_1w_x64(T[0], 50);
  mul_EltFp25519_1w_x64(T[0], T[0], T[3]);
  copy_EltFp25519_1w_x64(T[2], T[0]);
  sqrn_EltFp25519_1w_x64(T[2], 100);
  mul_EltFp25519_1w_x64(T[2], T[2], T[0]);
  sqrn_EltFp25519_1w_x64(T[2], 50);
  mul_EltFp25519_1w_x64(T[2], T[2], T[3]);
  sqrn_EltFp25519_1w_x64(T[2], 5);
  mul_EltFp25519_1w_x64(T[1], T[1], T[2]);
#undef sqrn_EltFp25519_1w_x64
}

inline void fred_EltFp25519_1w_x64(uint64_t *const c) {
  int64_t last = (((int64_t*)c)[3])>>63;
  c[3] &= ((uint64_t)1<<63)-1;
  c[0] += 19 & last;
}
