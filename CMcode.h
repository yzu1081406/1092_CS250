#ifndef CMCODE_H
#define CMCODE_H
#include<iostream>
#include<bitset>
#include<string>
#include<vector>
#include"Functionname.h"
#include"Functiontype.h"

using namespace std;
class CMcode {
public:
	CMcode();
	CMcode(int functname,string in);
	int getFunctiontType();
	int getFunctionName();
	int getRs1();
	int getRs2();
	int getRd();
	bitset<32> getMcode();
private:
	int fname;			//function name
	int ftype;			//function type
	int rs1, rs2, rd;	//register  and parameter
	bitset<32>Mcode;	//the output machinecode

	//Transform all the data members into machinecode. Only call the corresponding
	//function to get the output, not the actual workhorse.
	void FunctionToMachinecode();

	//use the function name in integer form and
	//set the ftype to the corressponding type
	//only Itype, Rtype, Stype are available
	void setFunctionType();

	//Input the RISC-V function in string to find the number of register.
	//e.g. addi x23,x21,3 => rd = 23,rs1 = 21, rs2 = 3
	//uses for loop to find the number,transform the string into the integer
	//		 rs1|rs2|rd
	//R-type:rs1|rs2|rd
	//I-type:rs1|imm|rd
	//S-type:rs1|imm|rs2
	void setRegisterNum(string in);

	//Transform the Rtype Instruction into machine code, following the
	//RISC-V model of instruction
	//31  27|26	 25|24	 20|19	 15|14	12|11	7|6    0|
	//funct5|funct2|  rs2  |  rs1  |funct3|	 rd  |opcode|
	void RTypeInstruction();

	//Transform the Stype Instruction into machine code, following the
	//RISC-V model of instruction
	//31     25|24   20|19   15|14  12|11     7|6    0|
	//imm[11:5]|  rs2  |  rs1  |funct3|imm[4:0]|opcode|
	void STypeInstruction();

	//Transform the Itype Instruction into machine code, following the
	//RISC-V model of instruction
	//31     20|19   15|14  12|11   7|6    0|
	//imm[11:0]|  rs1  |funct3|  rd  |opcode|
	void ITypeInstruction();
};
#endif