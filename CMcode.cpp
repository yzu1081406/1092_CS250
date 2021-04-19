#include "CMcode.h"

//constructor with no use
CMcode::CMcode()
{
	fname = -1;
	ftype = -1;
	rs1 = 0;
	rs2 = 0;
	rd = 0;
}

//constructor that will execute and transform Mcode into machinecode
CMcode::CMcode(int functname, string in)
{
	fname = functname;
	setFunctionType();
	setRegisterNum(in);
	FunctionToMachinecode();
}

void CMcode::FunctionToMachinecode()
{
	switch (ftype) {
	case Rtype:
		RTypeInstruction();
		break;
	case Itype:
		ITypeInstruction();
		break;
	case Stype:
		STypeInstruction();
		break;
	default:
		cout << "Function cannot be transform into machine code." << endl;
		break;
	}
}


void CMcode::setFunctionType()
{
	switch (fname) {
	case LB:
	case LH:
	case LW:
	case LBU:
	case LHU:
	case ADDI:
	case SLTI:
	case SLTIU:
	case XORI:
	case ORI:
	case ANDI:
		ftype = Itype;
		break;
	case SLLI:
	case SRLI:
	case SRAI:
	case ADD:
	case SUB:
	case SLL:
	case SLT:
	case SLTU:
	case XOR:
	case SRL:
	case SRA:
	case OR:
	case AND:
		ftype = Rtype;
		break;
	case SB:
	case SH:
	case SW:
		ftype = Stype;
		break;
	default:
		cout << "Not R,S,I type instruction." << endl;
		ftype = -1;
		break;
	}
}

void CMcode::setRegisterNum(string in)
{
	string temp;
	vector<string>allreg;
	int pos = 0;
	//find the ' ' after function name
	for (unsigned int i = 4; in[i] != ' '; i++) {
		pos = i;
	}
	//find the ',' and put every string before the comma
	//into a vector then clear the string
	for (unsigned int i = pos + 2; i < in.size(); i++) {
		if (in[i] == ',') {
			allreg.push_back(temp);
			temp.clear();
			i++;
		}
		temp.push_back(in[i]);
	}
	//put the last string in the vector
	allreg.push_back(temp);
	switch (ftype) {
	case Rtype:
	case Itype:
		for (unsigned int i = 0; i < allreg.size(); i++) {
			int regvalue;
			if (allreg[i][0] == 'x') {					//cut out x from the string
				regvalue = stoi(allreg[i].substr(1));	//transform x into an integer(e.g. x23 => 23)
			}
			else {
				regvalue = stoi(allreg[i]);				//transform string into an integer(Itype immediate)
			}
			//put the value into the reference in the right position
			switch (i) {
			case 0:
				rd = regvalue;	//in Rtype,Itype rd = rd
				break;
			case 1:
				rs1 = regvalue;	//in Rtype,Itype rs1 = rs1
				break;
			case 2:
				rs2 = regvalue; //in Rtype rs2 = rs2, Itype rs2 = imm[11:0]
				break;
			}
		}
		break;
	case Stype:
		for (unsigned int i = 0; i < allreg.size(); i++) {
			int regvalue;
			if (allreg[i][0] == 'x') {					//cut out x from the string	
				regvalue = stoi(allreg[i].substr(1));
				rd = regvalue;							//rd is rs2 in the instruction
			}
			else {
				int lpos = 0;
				int rpos = 0;
				bool check = 1;
				for (unsigned int j = 0; j < allreg[i].size(); j++) {
					if (allreg[i][j] != '(' && check) {		//find the last position before '('
						lpos = j;
					}
					else {
						check = 0;
					}
					if (allreg[i][j] != ')') {				//find the last position before ')'
						rpos = j;
					}
				}
				if (lpos == 0) {									//before '(' only one number
					regvalue = allreg[i][0] - '0';
				}
				else {												//before '(' more than one number
					regvalue = stoi(allreg[i].substr(0, lpos));
				}
				rs2 = regvalue;										//rs2 is offset imm in the instruction
				regvalue = stoi(allreg[i].substr(lpos + 3, rpos));	//after '(x' before ')', cut out the string in between 
				rs1 = regvalue;										//rs1 is rs1 in the instruction
			}
		}
		break;
	default:
		break;
	}
}


void CMcode::RTypeInstruction()
{
	//opcode = 0010011
	Mcode.set(0);
	Mcode.set(1);
	Mcode.set(4);

	//set the funct5|funct2,funct3,opcode
	switch (fname) {
		//funct5|function2 = 00000|00 by defualt
	case SLLI:
		//opcode = 0010011
		//funct3 = 001
		Mcode.set(12);
		break;
	case SRLI:
		//funct3 = 101
		Mcode.set(12);
		Mcode.set(14);
		break;
	case SRAI:
		//funct5|funct2 = 01000|00
		Mcode.set(30);
		//funct3 = 101
		Mcode.set(12);
		Mcode.set(14);
		break;
	case ADD:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 000
		break;
	case SUB:
		//funct5|funct2 = 01000|00
		Mcode.set(30);
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 000
		break;
	case SLL:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 001
		Mcode.set(12);
		break;
	case SLT:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 010
		Mcode.set(13);
		break;
	case SLTU:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 011
		Mcode.set(12);
		Mcode.set(13);
		break;
	case XOR:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 100
		Mcode.set(14);
		break;
	case SRL:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 101
		Mcode.set(12);
		Mcode.set(14);
	case SRA:
		//funct5|funct2 = 01000|00
		Mcode.set(30);
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 101
		Mcode.set(12);
		Mcode.set(14);
		break;
	case OR:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 110
		Mcode.set(13);
		Mcode.set(14);
		break;
	case AND:
		//opcode = 0110011
		Mcode.set(5);
		//funct3 = 111
		Mcode.set(12);
		Mcode.set(13);
		Mcode.set(14);
		break;
	default:
		cout << "Not Rtype instruction Error" << endl;
		break;
	}
	bitset<5>bitrs1(rs1);
	bitset<5>bitrs2(rs2);
	bitset<5>bitrd(rd);
	//set the rd
	for (int k = 7, p = 0; k <= 11; k++, p++) {
		Mcode[k] = bitrd[p];
	}
	//set the rs1
	for (int k = 15, p = 0; k <= 19; k++, p++) {
		Mcode[k] = bitrs1[p];
	}
	//set the rs2
	for (int k = 20, p = 0; k <= 24; k++, p++) {
		Mcode[k] = bitrs2[p];
	}
}


void CMcode::ITypeInstruction()
{
	//opcode = 0000011	
	Mcode.set(0);
	Mcode.set(1);
	//set the funct3,opcode
	switch (fname) {
	case LB:
		//opcode = 0000011 by defualt	
		//funct3 = 000
		break;
	case LH:
		//funct3 = 001
		Mcode.set(12);
		break;
	case LW:
		//funct3 = 010
		Mcode.set(13);
		break;
	case LBU:
		//funct3 = 100
		Mcode.set(14);
		break;
	case LHU:
		//funct3 = 101
		Mcode.set(12);
		Mcode.set(14);
		break;
	case ADDI:
		//opcode = 0010011
		Mcode.set(4);
		//funct3 = 000
		break;
	case SLTI:
		//opcode = 0010011
		Mcode.set(4);
		//funct3 = 010
		Mcode.set(13);
		break;
	case SLTIU:
		//opcode = 0010011
		Mcode.set(4);
		//funct3 = 011
		Mcode.set(12);
		Mcode.set(13);
		break;
	case XORI:
		//opcode = 0010011
		Mcode.set(4);
		//funct3 = 100
		Mcode.set(14);
		break;
	case ORI:
		//opcode = 0010011
		Mcode.set(4);
		//funct3 = 110
		Mcode.set(13);
		Mcode.set(14);
		break;
	case ANDI:
		//opcode = 0010011
		Mcode.set(4);
		//funct3 = 110
		Mcode.set(13);
		Mcode.set(14);
		break;
	default:
		cout << "Not Rtype instruction Error" << endl;
		break;
	}
	bitset<5>bitrs1(rs1);
	bitset<5>bitrd(rd);
	bitset<12>bitimm(rs2);
	//set the rd
	for (int i = 7, j = 0; i <= 11; i++, j++) {
		Mcode[i] = bitrd[j];
	}
	//set the rs1
	for (int i = 15, j = 0; i <= 19; i++, j++) {
		Mcode[i] = bitrs1[j];
	}
	//set the imm[11:0]
	for (int i = 20, j = 0; i <= 31; i++, j++) {
		Mcode[i] = bitimm[j];
	}
}


void CMcode::STypeInstruction()
{
	//opcode = 0100011
	Mcode.set(0);
	Mcode.set(1);
	Mcode.set(5);
	//set the funct3,opcode
	switch (fname) {
	case SB:
		//funct3 = 000
		break;
	case SH:
		//funct3 = 001
		Mcode.set(12);
		break;
	case SW:
		Mcode.set(13);
		break;
	}
	bitset<5>bitrs1(rs1);
	bitset<5>bitrs2(rd);
	bitset<11>bitimm(rs2);
	//set the imm[4:0]
	for (int i = 7, j = 0; i <= 11; i++, j++) {
		Mcode[i] = bitimm[j];
	}
	//set the rs1
	for (int i = 15, j = 0; i <= 19; i++, j++) {
		Mcode[i] = bitrs1[j];
	}
	//set the rs2
	for (int i = 20, j = 0; i <= 24; i++, j++) {
		Mcode[i] = bitrs2[j];
	}
	//set the imm[11:5]
	for (int i = 25, j = 0; i <= 31; i++, j++) {
		Mcode[i] = bitimm[j];
	}
}

int CMcode::getFunctiontType()
{
	return ftype;
}

int CMcode::getFunctionName()
{
	return fname;
}

int CMcode::getRs1()
{
	return rs1;
}

int CMcode::getRs2()
{
	return rs2;
}

int CMcode::getRd()
{
	return rd;
}

bitset<32> CMcode::getMcode()
{
	return bitset<32>(Mcode);
}
