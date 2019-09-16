#include <fstream>
#include <math.h>
#include <Windows.h>
#include <wincrypt.h>

using namespace std;

unsigned x0, x1, x2;
unsigned const M_F = ((unsigned)(-5));

void randomInit(unsigned a, unsigned b, unsigned c)
{
	x0 = a;
	x1 = b;
	x2 = c;
}

unsigned random() 
{	
	unsigned long long x3 = (x0 + x1 + x2) % M_F;
	for (int i = 0; i < 13; i++)
		x3 = (x3 << 1) % M_F;
	x0 = x1;
	x1 = x2;
	x2 = x3;
	return x3;
}

int main(int argc, char *argv[])
{
	//Standart rand function
	ofstream fout1, fout2, fout3;
	fout1.open("randomStandart.txt", ios::trunc | ios::binary);
	for (unsigned i = 0; i < ((unsigned)1 << 23); i++) 
	{
		unsigned r = rand();
		fout1.write(reinterpret_cast<const char*>(&r), sizeof(r));
	}
	fout1.close();

	//Crypto function  
	DWORD cbGoop = (unsigned)1 << 24;
	BYTE* lpGoop = new BYTE[(unsigned)1 << 24];
	HCRYPTPROV m_hProv;
	m_hProv = NULL;
	::CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	fout2.open("randomCrypt.txt", ios::trunc | ios::binary);
	CryptGenRandom(m_hProv, cbGoop, lpGoop);
	fout2.write((char*)lpGoop, (unsigned)1 << 24);
	fout2.close();
	if (m_hProv) ::CryptReleaseContext(m_hProv, 0);

	//Custom algoritm
	unsigned a = 3, b = 7, c = 13;
	randomInit(a, b, c);
	fout3.open("randomCustom.txt", ios::trunc | ios::binary);
	for (unsigned i = 0; i < ((unsigned)1 << 23); i++) 
	{
		unsigned rrr = random();
		fout3.write(reinterpret_cast<const char*>(&rrr), sizeof(rrr));
	}
	fout3.close();
}

