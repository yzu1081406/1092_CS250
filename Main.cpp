#include<iostream>
#include<string>
#include<bitset>
#include"CMcode.h"
using namespace std;

//hashing string using constexpr to switch in compile time
//reference:
/*————————————————
版权声明：本文为CSDN博主「YoziDream」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https ://blog.csdn.net/yozidream/article/details/22789147*/
typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;
//input a char const* and return a hash value for the switch()
hash_t hash_(char const* str)
{
	hash_t ret{ basis };

	while (*str) {
		ret ^= *str;
		ret *= prime;
		str++;
	}

	return ret;
};
//input a char const* and return a constexpr for the switch case in compile time
//using recursive call for the constexpr
constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
{
	return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}
//easier to use the hash function for the switch case 
constexpr unsigned long long operator "" _hash(char const* p, size_t)
{
	return hash_compile_time(p);
}

string getFunct(string in);
int getFunctionName(string funct);


int main() {
	string input;
	while (getline(cin, input) && input != "`") {

		int p = 0;

		string funct = getFunct(input);				//cut out the part of the function for easier use
		int myfname = getFunctionName(funct);		//switching the function name into an integer for easier use
		
		CMcode mymachinecode(myfname,input);		//workhorse
		
		cout << mymachinecode.getMcode() << endl;
	}
}

//Input the RISC-V function and return the function part of the string
//e.g. in =  add x2 x3 x4 => return add
string getFunct(string in) {
	string temp = "";
	for (unsigned int i = 4; i < in.size() && in[i] != ' ';i++) {
		temp.push_back(in[i]);
	}
	return temp;
}

//Input the RISC-V function in string and return an integer corressponding to 
//the function name for later uses.
//using hash function and constexpr to switch the string in compile time
int getFunctionName(string funct) {
	const char* temp = funct.c_str();
	int myfunctionname;
	switch (hash_(temp)) {
	case "lui"_hash:
		myfunctionname = LUI;
		break;
	case"auipc"_hash:
		myfunctionname = AUIPC;
		break;
	case"jal"_hash:
		myfunctionname = JAL;
		break;
	case"jalr"_hash:
		myfunctionname = JALR;
		break;
	case"beq"_hash:
		myfunctionname = BEQ;
		break;
	case"bne"_hash:
		myfunctionname = BNE;
		break;
	case"blt"_hash:
		myfunctionname = BLT;
		break;
	case"bge"_hash:
		myfunctionname = BGE;
		break;
	case"bltu"_hash:
		myfunctionname = BLTU;
		break;
	case"bgeu"_hash:
		myfunctionname = BGEU;
		break;
	case"lb"_hash:
		myfunctionname = LB;
		break;
	case"lh"_hash:
		myfunctionname = LH;
		break;
	case"lw"_hash:
		myfunctionname = LW;
		break;
	case"lbu"_hash:
		myfunctionname = LBU;
		break;
	case"lhu"_hash:
		myfunctionname = LHU;
		break;
	case"sb"_hash:
		myfunctionname = SB;
		break;
	case"sh"_hash:
		myfunctionname = SH;
		break;
	case"sw"_hash:
		myfunctionname = SW;
		break;
	case"addi"_hash:
		myfunctionname = ADDI;
		break;
	case"slti"_hash:
		myfunctionname = SLTI;
		break;
	case"sltiu"_hash:
		myfunctionname = SLTIU;
		break;
	case"xori"_hash:
		myfunctionname = XORI;
		break;
	case"ori"_hash:
		myfunctionname = ORI;
		break;
	case"andi"_hash:
		myfunctionname = ANDI;
		break;
	case"slli"_hash:
		myfunctionname = SLLI;
		break;
	case"srli"_hash:
		myfunctionname = SRLI;
		break;
	case"srai"_hash:
		myfunctionname = SRAI;
		break;
	case"add"_hash:
		myfunctionname = ADD;
		break;
	case"sub"_hash:
		myfunctionname = SUB;
		break;
	case"sll"_hash:
		myfunctionname = SLL;
		break;
	case"slt"_hash:
		myfunctionname = SLT;
		break;
	case"sltu"_hash:
		myfunctionname = SLTU;
		break;
	case"xor"_hash:
		myfunctionname = XOR;
		break;
	case"srl"_hash:
		myfunctionname = SRL;
		break;
	case"sra"_hash:
		myfunctionname = SRA;
		break;
	case"or"_hash:
		myfunctionname = OR;
		break;
	case"and"_hash:
		myfunctionname = AND;
		break;
	default:
		cout << "instruction not in set" << endl;
		myfunctionname = -1;
		break;
	}
	return myfunctionname;
}
