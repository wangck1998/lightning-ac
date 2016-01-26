#define MemoryPool 0x100000 * 10

#ifndef ONLINE_JUDGE
#define errorp(...) fprintf(stderr, __VA_ARGS__)
#else
#define errorp(...) ;
#endif

long long m[MemoryPool];

enum {
  LOD = 100, IMM, LEA, JMP, JZ, 
  JNZ, STO, PSH, ENT, LEV,
  CALL,EXIT, 
};

enum operation {
  ADD = 200, SUB, MUL, DIV, MOD, 
  EQ, NE, LT, GT, LE, 
  GE, SHL, SHR
};

enum systemFunction {
  RINT = 300, WINT, GETC, PUTC
};


int VMRun(const long long* const text, int lenText, long long idMain) {
  long long i;
  for (i = 0; i < lenText; i++) m[i] = text[i];
  
  long long cur = idMain;
  long long bp, sp = MemoryPool - 1; // stack pointer
  long long reg; // only one register
  
  while (1) {
    i = m[cur++]; // instruction
    // load 
    if      (i == LOD) reg = m[reg];
    // load immediately or global address
    else if (i == IMM) reg = m[cur++];
    // load local address
    else if (i == LEA) reg = bp - m[cur++];
    // jump
    else if (i == JMP) cur = m[cur];
    // jump equal zero
    else if (i == JZ) if (!reg) cur = m[cur]; else cur++;
    // jump not equal zero
    else if (i == JNZ) if (reg) cur = m[cur]; else cur++;
    // store 
    else if (i == STO) m[m[cur]++] = reg;
    // push
    else if (i == PSH) m[--sp] = reg;
    // enter subroutine
    else if (i == ENT) {m[--sp] = bp; bp = sp; sp -= m[cur++];}
    // leave subroutine
    else if (i == LEV) {sp = bp, bp = m[++sp]; cur = m[++sp];}
    // jump to subroutine, call function
    else if (i == CALL) {m[--sp] = cur; cur = m[cur];}
    // exit
    else if (i == EXIT) {errorp("exit(%lld)\n", reg); break;}

    // ==================== operation   ======================
    else if (i == ADD) reg = m[sp++] + reg;
    else if (i == SUB) reg = m[sp++] - reg;
    else if (i == MUL) reg = m[sp++] * reg;
    else if (i == DIV) reg = m[sp++] / reg;
    else if (i == MOD) reg = m[sp++] % reg;
    else if (i == EQ) reg = m[sp++] == reg;
    else if (i == NE) reg = m[sp++] != reg;
    else if (i == LT) reg = m[sp++] < reg;
    else if (i == GT) reg = m[sp++] > reg;
    else if (i == LE) reg = m[sp++] <= reg;
    else if (i == GE) reg = m[sp++] >= reg;
    else if (i == SHL) reg = m[sp++] << reg;
    else if (i == SHR) reg = m[sp++] >> reg;

    // ==================== system call ======================
    else if (i == RINT) scanf("%lld", &reg);
    else if (i == WINT) printf("%lld", reg);
    else if (i == GETC) reg = getchar();
    else if (i == PUTC) putchar(reg);
    else {errorp("unknown instruction = %lld\n", i); return -1;}
  }

  return 0;
}