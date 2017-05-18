#include <iostream>
#include <string>
#include "hash.h"
#include "third_party/freelip-1.1/lip.h"
#include "constdef.h"

static char modulus_n[] = "A70F8F62AA6E97D1";//模数
static char d_str[] = "A7CAD9177AE95A9";//私钥
static char op_str5[] = "10001";//公钥
//static char check_str6[] = "55776B";
static char UTRISO_str[] = "UTRISO";
//static char REGCODE_str[] = "4BA90D54214AC938";
void RSA_decrypt(char* encrypted_data)
{
    std::string regcode;
    regcode.resize(64);

    long* m = nullptr;       
    long* n = nullptr;
    long* result = nullptr;
    long* d = nullptr;
    zhsread(encrypted_data, &m);
    zhsread(modulus_n, &n);
    zhsread(d_str, &d);
    zexpmod(m, d, n, &result);
    zhswrite(const_cast<char*>(regcode.c_str()), result);
    std::cout << regcode << " len:" << strlen(regcode.c_str()) << std::endl;
}

void RSA_encrypt(char* plaintext)
{
    long* a = nullptr;
    long* b = nullptr;
    long* c = nullptr;
    long* d = nullptr;
    zhsread(plaintext, &a);
    zhsread(modulus_n, &c);
    zhsread(op_str5, &b);
    zexpmod(a, b, c, &d);//d=(a^b)%c
    std::string e;
    e.resize(64);
    zhswrite(const_cast<char*>(e.c_str()), d);

}
void validate()
{
	long* a = nullptr;
	long* b = nullptr;
	long* c = nullptr;
	long* d = nullptr;
	auto flag = 0;//控制最终注册码是否正确
	std::string operated_regcode;
	operated_regcode.resize(16);
	//auto seat_num = 0;
	//auto seat_c = 0;

	std::string regcode;
	std::string name;
	std::cout << "Registration Name: " << std::endl;
	while (std::getline(std::cin, name, '\n')) {
		name.shrink_to_fit();
		std::cout << "Registration Code: " << std::endl;
		std::getline(std::cin, regcode, '\n');
		size_t pos;
		//去除注册码里的所有'-'
		while (pos = regcode.find("-"), pos != std::string::npos)
		{
			regcode.replace(pos, 1, "");
		}
		std::cout << "Registration Code: " << regcode<<std::endl;

		HASH hasher;

		zhsread(const_cast<char*>(regcode.c_str()), &a);
		zhsread(modulus_n, &c);//C=模数
		zhsread(op_str5, &b);//b=公钥
		zexpmod(a, b, c, &d);//d=(a**b)%c，加密注册码
		zhswrite(const_cast<char*>(operated_regcode.c_str()), d);//operated_regcode=HEX(d)
		std::cout << "RSA-64 encrypted registration code: " << operated_regcode << std::endl;
		hasher.lowerStr(const_cast<char*>(operated_regcode.c_str()));

/*
m必须满足
m[0]==4|5
m[1]==5|6
m[6]==2|d
m[7]==a|b|c
m[8]==5
m[9]==3
m总共16位.
*/
		if (operated_regcode[0] != '5' &&
			operated_regcode[0] != '4')
		{
			if (operated_regcode[0] != '7' &&
				operated_regcode[1] != '7')
			{
				if (operated_regcode[0] == '6' ||
					operated_regcode[1] == 'B')
				{
					flag += 4;
					/*if (operated_regcode[14] < 'a')
						seat_c = operated_regcode[14] - '0';
					else
						seat_c = operated_regcode[14] - 'W';
					seat_c *= 16;
					if (operated_regcode[15] < 'a')
						seat_c += operated_regcode[15] - '0';
					else
						seat_c += operated_regcode[15] - 'W';
					//seat_c -= 32;
					if (operated_regcode[8] < 'a')
						seat_num = operated_regcode[8] - '0';
					else
						seat_num = operated_regcode[8] - 'W';
					seat_num *= 16;
					if (operated_regcode[9] < 'a')
						seat_num += operated_regcode[9] - '0';
					else
						seat_num += operated_regcode[9] - 'W';
					seat_num -= 32;
					seat_num += seat_c << 6;*/
				}
			}
			else
			{
				flag += 4;
				/*if (operated_regcode[8] < 97)
					seat_num = operated_regcode[8] - '0';
				else
					seat_num = operated_regcode[8] - 'W';
				seat_num *= 16;
				if (operated_regcode[9] < 97)
					seat_num += operated_regcode[9] - '0';
				else
					seat_num += operated_regcode[9] - 'W';
				seat_num -= 32;*/
			}
		}
		else
		{

			++flag;
			if (operated_regcode[1] == '5' || operated_regcode[1] == '6')
				++flag;
			if (operated_regcode[8] == '5')
				++flag;
			if (operated_regcode[9] == '3')
				++flag;
		}
//然后连接"UTRISO",用户名,注册码
		std::string composite_str = "UTRISO" + name + regcode;
//计算k的MD5值
		std::string composite_md5 = hasher(composite_str.c_str(), "MD5");
		hasher.upperStr(const_cast<char*>(composite_md5.c_str()));
		std::cout << "composite UTRISO,name,regcode md5: " << composite_md5 << std::endl;
//取MD5值前6位与已存在的数据比较(0x0064C0C8处,数据的MD5:B2C2A396BE3864C4210519A8303FD1E5),有相同的就正确.
		unsigned int comp_md5_len;
		const unsigned char* hex_composite_md5 = hasher.hash(composite_str.c_str(), "MD5", comp_md5_len);
		if ((operated_regcode[6] < '2' || operated_regcode[6] > '2') && (operated_regcode[6] < 'd' || operated_regcode[6] > 'd'))
			--flag;
		if (operated_regcode[7] != 'a' && operated_regcode[7] != 'c' && operated_regcode[7] != 'b')
			--flag;
		flag -= 2;
		auto len_data = 52413;
		auto i = 0;
		auto index = 0;
		do
		{

			index = (len_data + i) >> 1;
			int result = memcmp(&data[6 * index], hex_composite_md5, 6);
			if (result <= 0)
			{
				if (result >= 0)
				{
					flag -= 2;
					if (flag == 0)
					{
						//success
						std::cout << "matched key check data at offset " << 6 * index << std::endl;
					}
					break;
				}
				i = index + 1;
			}
			else
			{
				len_data = index - 1;
			}
		} while (i <= len_data);
		std::cout << "last compared data: ";
		for (i = 0; i < 6; ++i)
		{
			printf("%02X", data[6 * index + i]);
		}
		std::cout << std::endl;




		std::string check_data_md5 = hasher(reinterpret_cast<const char*>(data), "MD5", 314484);
		hasher.upperStr(const_cast<char*>(check_data_md5.c_str()));
		std::cout << "key check data md5: " << check_data_md5 << std::endl;

		if (flag == 0)
		{
			std::cout << "correct registration!" << std::endl;
		}
		else
		{
			std::cout << "wrong registration!" << std::endl;
		}
		FREESPACE(a);
		FREESPACE(b);
		FREESPACE(c);
		FREESPACE(d);
		flag = 0;
		std::cout << "Registration Name: " << std::endl;
	}
}