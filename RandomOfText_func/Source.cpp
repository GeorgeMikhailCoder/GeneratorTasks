#include<fstream>
#include<iostream>
#include<clocale>
#include<ctime>
using namespace std;

int SumMass(int *mass, int N)
{
	int sum = 0;
	for (int i = 0; i < N; i++)
		sum += mass[i];
	return sum;
}

int NumMax(int *mass, int N)
{
	int max = mass[0];
	int i = 1;
	int k = 0;
	for (; i < N; i++)
		if (mass[i]>max)
		{
			max = mass[i];
			k = i;
		}
	return k;
}

int Max(int *mass, int N)
{
	int max = mass[0];

	for (int i = 1; i < N; i++)
		if (mass[i]>max)max = mass[i];
	return max;
}

int NumMin(int *mass, int N)
{
	int min = mass[0];
	int i = 1;
	int k = 0;
	for (; i < N; i++)
		if (mass[i] < min)
		{
			min = mass[i];
			k = i;
		}
	return k;
}

int Min(int *mass, int N)
{
	int min = mass[0];

	for (int i = 1; i < N; i++)
		if (mass[i]<min)min = mass[i];
	return min;
}

bool CompareElem(int *mass, int N, int Elem)
{
	bool b = 0;
	for (int i = 0; i < N; i++)
		if (mass[i] == Elem)
		{
			b = 1;
			break;
		}
	return b;
}

int GetKolvoBlok(FILE *Base)
{
	int KolvoBlok = 0;
	{
		int a = getc(Base);
		if (a != 0)KolvoBlok++;
		rewind(Base);
	}

	//cout << "STR: ";
	char str[1000];
	for (; !feof(Base);)
	{
		fgets(str, 1000, Base);
		//cout << str;
		if (str[0] == '\n') KolvoBlok++;
	}

	rewind(Base);

	return KolvoBlok;
}

void GetKolvoStr(FILE *Base, int *KolvoStr, int KolvoBlok)
{
	char str[1000];
	for (int k = 0; !feof(Base);)
	{

		for (int i = 0; i < KolvoBlok; )
		{

			fgets(str, 1000, Base);
			k++;

			if (str[0] == '\n' || feof(Base))
			{
				KolvoStr[i] = k - 1 + feof(Base);
				k = 0;
				i++;
			}
		}

	}
	rewind(Base);

}

int EnterKolvoStr(int SumKolStr)
{
	int NewSumKolStr;
	cout << "Введите количество строк: " << endl;
	cin >> NewSumKolStr;
	//NewSumKolStr = 20;
	for (; NewSumKolStr > SumKolStr;)
	{
		cout << "Количество строк больше базы! Введите число, меньше " << SumKolStr << endl;
		cin >> NewSumKolStr;
	}
	return NewSumKolStr;
}

void EnterParam(bool &poryadokOfBlok, bool &zeliyBlok)
{
	cout << "Сохранить целостность блоков? (1 - да, 0 - нет)" << endl;
	cin >> zeliyBlok;
	if (zeliyBlok == 1)
	{
		cout << "Сохранить порядок блоков? (1 - да, 0 - нет)" << endl;
		cin >> poryadokOfBlok;
	}
	else
		poryadokOfBlok = 0;

}

void SetKolStrFromBlok(int *KolStrFromBlok, int * KolvoStr, int NewSumKolStr,int SumKolStr,int KolvoBlok)
{
	{
		float k = (float)NewSumKolStr / SumKolStr;
		for (int i = 0; i < KolvoBlok; i++)
		{
			KolStrFromBlok[i] = k*KolvoStr[i] + i % 2;
			//cout << KolStrFromBlok[i] << "\t";
		}
	}
	//cout << "Сумма : " << SumMass(KolStrFromBlok, KolvoBlok) << endl;
	//выравнивание: если меньше, добавить к последнему минимальному блоку, если больше, убрать из последнего максимального
	for (; (SumMass(KolStrFromBlok, KolvoBlok) > NewSumKolStr); KolStrFromBlok[NumMax(KolStrFromBlok, KolvoBlok)]--);
	for (; (SumMass(KolStrFromBlok, KolvoBlok) < NewSumKolStr); KolStrFromBlok[NumMin(KolStrFromBlok, KolvoBlok)]++);

}

void SetMatrixOfRandom(int **matrixOfRandom,int *KolStrFromBlok, int *KolvoStr, int KolvoBlok)
{
	for (int i = 0; i < KolvoBlok; i++) matrixOfRandom[i] = new int[Max(KolStrFromBlok, KolvoBlok)];

	// заполнение -1:
	for (int i = 0; i < KolvoBlok; i++)
		for (int j = 0; j < Max(KolStrFromBlok, KolvoBlok); j++)
			matrixOfRandom[i][j] = -1;

	int heigh = KolvoBlok;
	int length = Max(KolStrFromBlok, KolvoBlok);

	//cout << "RandomMatrix:" << endl;

	srand(time(NULL));
	int r = 0;
	for (int i = 0; i < heigh; i++)
	{
		for (int j = 0; j < KolStrFromBlok[i]; j++)
		{
			r = rand() % KolvoStr[i];

			for (; CompareElem(matrixOfRandom[i], length, r); r = rand() % KolvoStr[i]);

			matrixOfRandom[i][j] = r;
			//cout << matrixOfRandom[i][j] << "\t";
		}
		//cout << endl;
	}
}

void SetMatrixOfRandomBlok(int *matrixOfRandomBlok,int KolvoBlok)
{
	int r;
	for (int i = 0; i < KolvoBlok; i++)
		matrixOfRandomBlok[i] = -1;

	//cout << "MatrixOfRandomBlok = " << endl;
	for (int i = 0; i < KolvoBlok; i++)
	{
		r = rand() % KolvoBlok;
		for (; CompareElem(matrixOfRandomBlok, KolvoBlok, r); r = (r + rand()) % KolvoBlok);
		matrixOfRandomBlok[i] = r;
		//cout << matrixOfRandomBlok[i] << "\t";
	}
}

int GetMaxStrLen(FILE *Base, int *KolvoStr, int SumKolStr,int KolvoBlok)
{
	rewind(Base);
	int *StrLen = new int[SumKolStr];
	int MaxLenStr = 0;
	for (int i = 0; i < KolvoBlok; i++)
	{
		for (int j = 0; j < KolvoStr[i]; j++)
		{

			char c = '0';
			int k = 0;
			for (; c != '\n' && !feof(Base); k++)
				c = fgetc(Base);
			k > MaxLenStr ? MaxLenStr = k : k = k;
		}
		fgetc(Base);
	}
	MaxLenStr++; // т.к. знак '\n'
	return MaxLenStr;
}

void CreateResultFile(FILE *Result, FILE *Base, bool poryadokOfBlok, bool zeliyBlok, 
					  int KolvoBlok, int *KolvoStr, int *KolStrFromBlok, int MaxLenStr,
						int *matrixOfRandomBlok, int **matrixOfRandom)
{
	//cout << "ResultFile: " << endl;
	char* Str = new char[MaxLenStr];
	int k = 1;
	int ispstr = 0;
	//for (;k<SumMass(KolStrFromBlok,KolvoBlok);)
	for (int i = 0; i < KolvoBlok; i++)
	{
		for (int j = 0; j < KolStrFromBlok[poryadokOfBlok == 0 ? matrixOfRandomBlok[i] : i]; j++)
		{
			rewind(Base);
			for (int x = 0; x < MaxLenStr; x++)Str[x] = '\n';

			//cout<<endl << "Blok # " << i << "  Stroka In Result # " << j << " In Base #" << matrixOfRandom[i][j] << endl;
			//cout << "IspStr = "<<ispstr<<"  Str in Base # " << ispstr + matrixOfRandom[i][j] << " In Result # " << k << endl<<endl;


			ispstr = 0;
			if (poryadokOfBlok == 0)
				for (int p = 0; p < matrixOfRandomBlok[i]; p++) ispstr += (KolvoStr[p] + 1);
			else
				for (int p = 0; p < i; p++) ispstr += (KolvoStr[p] + 1);

			if (poryadokOfBlok == 0)
				ispstr += matrixOfRandom[matrixOfRandomBlok[i]][j];
			else
				ispstr += matrixOfRandom[i][j];

			//cout << "k =  " << k << " RandBlok = " << matrixOfRandomBlok[i] << " Str = " << ispstr << endl;


			for (int l = 0; l <= ispstr; l++) fgets(Str, MaxLenStr, Base);
			fprintf(Result, "%d) ", k);
			fputs(Str, Result);
			k++;
			if (zeliyBlok == 0)break;
			//cout << Str;
		}
		if (zeliyBlok == 0 && i == KolvoBlok - 1)	i = -1;

		if (k == SumMass(KolStrFromBlok, KolvoBlok) + 1)break;
	}
}

void main()
{
	setlocale(0, "Russian");
	FILE *Base;
	fopen_s(&Base, "BaseOfData.txt", "r");
	if (Base == NULL)
		cout << "There is no Base Of Data!" << endl;
	else
	{
	
		int KolvoBlok = GetKolvoBlok(Base);
		int *KolvoStr = new int[KolvoBlok];
		GetKolvoStr(Base, KolvoStr, KolvoBlok);
		int SumKolStr = SumMass(KolvoStr, KolvoBlok);
		int NewSumKolStr = EnterKolvoStr(SumKolStr);
		bool poryadokOfBlok = 0;
		bool zeliyBlok = 0;
		EnterParam(poryadokOfBlok, zeliyBlok);
		int *KolStrFromBlok = new int[KolvoBlok];
		SetKolStrFromBlok(KolStrFromBlok, KolvoStr, NewSumKolStr, SumKolStr, KolvoBlok);
		int **matrixOfRandom = new int*[KolvoBlok];
		SetMatrixOfRandom(matrixOfRandom, KolStrFromBlok, KolvoStr, KolvoBlok);
		int *matrixOfRandomBlok = new int[KolvoBlok];
		SetMatrixOfRandomBlok(matrixOfRandomBlok, KolvoBlok);
		int MaxLenStr = GetMaxStrLen(Base, KolvoStr, SumKolStr, KolvoBlok);		
		
		rewind(Base);
		FILE *Result;
		fopen_s(&Result, "Result.txt", "w");
		if (Result == NULL)
			cout << "Can't create a file!" << endl;
		else
			CreateResultFile(Result, Base, poryadokOfBlok, zeliyBlok, KolvoBlok, KolvoStr, KolStrFromBlok, MaxLenStr, matrixOfRandomBlok, matrixOfRandom);
		
		
		//////////////////////////////////Вывод:
		//  cout << "kolvoBlok = " << KolvoBlok << endl << "KolvoStr = " << endl;
		//  for (int i = 0; i < KolvoBlok; i++)
		//  	cout << KolvoStr[i] << "\t";
		//  cout << "\nСумма : " << SumMass(KolvoStr, KolvoBlok) << endl;
		//  cout << "KolvoStrFromBlok = " << endl;
		//  for (int i = 0; i < KolvoBlok; i++)
		//  	cout << KolStrFromBlok[i] << "\t";
		//  cout << "Сумма : " << SumMass(KolStrFromBlok, KolvoBlok) << endl;
		//  cout << endl << "MaxLenStr = " << MaxLenStr << endl;
		cout << "Result File has been done!"<<endl;
		////////////////////////////////Конец вывода.

		fclose(Base);
		fclose(Result);

		for (int i = 0; i < KolvoBlok; i++)
			delete[]matrixOfRandom[i];
		delete[] matrixOfRandom;

		delete[] matrixOfRandomBlok;
		delete[]KolStrFromBlok;
		delete[]KolvoStr;
	}
	system("pause");
}