#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>

using namespace std;

//��ȡ�ļ��� res ��
bool ReadTxt(const string& path, vector<char>& res)
{
	fstream file;
	file.open(path, ios::in);
	if (!file.is_open())
	{
		cout << "��ȡ�ļ�" << path << "ʧ��" << endl;
		return false;
	}
	vector<char> buf(file.seekg(0, ios::end).tellg());
	file.seekg(0, ios::beg).read(&buf[0], static_cast<streamsize>(buf.size()));

	file.close();
	res = buf;

	return true;
}

//ͳ�ƴ�Ƶ ��UTF-8������ʽ��
unordered_map<string, int> CaculateWordFrequency(const vector<char>& txt)
{
	unordered_map<string, int> resMap;
	string word;

	for (size_t i = 0; i < txt.size();)
	{
		size_t len = 1;									//0xxxxxxx
		if ((txt[i] & 0xf8) == 0xf0)len = 4;			//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if ((txt[i] & 0xf0) == 0xe0)len = 3;		//1110xxxx 10xxxxxx 10xxxxxx
		else if ((txt[i] & 0xe0) == 0xc0)len = 2;		//110xxxxx 10xxxxxx
		if (i + len > txt.size())len = 1;
		for (size_t j = 0; j < len; j++)
		{
			word += txt[j + i];
		}
		if (resMap.find(word) != resMap.end())resMap[word]++;	//�����иô�
		else resMap[word] = 1;									//����û��
		i += len;
		word.clear();
	}
	/*
	for (int i = 0; i < txt.size();)
	{
		if ((txt[i] & 0x80) && word.empty())			//����ͳ��һ���֣�gbk���룩
		{
			word.push_back(txt[i]);
			word.push_back(txt[i + 1]);
			i += 2;
		}
		else if ((txt[i] & 0x80)==0)					//Ӣ��ͳ��һ������
		{
			if (isalnum(txt[i]))
			{
				word.push_back(tolower(txt[i]));
				i++;
				continue;
			}
			else i++;
		}

		if (resMap.find(word) != resMap.end())resMap[word]++;
		else resMap[word] = 1;
		word.clear();
	}
	if (!word.empty())
	{
		if (resMap.find(word) != resMap.end())resMap[word]++;
		else resMap[word] = 1;
	}*/
	return resMap;
}

//�����������ƶ�
double CaculateSimilarity(const unordered_map<string, int>& freq1, const unordered_map<string, int>& freq2)
{
	double dot = 0, sqrMagn1 = 0, sqrMagn2 = 0;
	for (auto& pair : freq1)
	{
		if (freq2.find(pair.first) != freq2.end())			//����һƪ��Ҳ�������
			dot += pair.second * freq2.at(pair.first);
		sqrMagn1 += pair.second * pair.second;				//��һƪ�Ĵ�Ƶ��ƽ����
	}
	for (auto& pair : freq2)
	{
		sqrMagn2 += pair.second * pair.second;				//�ڶ�ƪ�Ĵ�Ƶ��ƽ����
	}
	if (sqrMagn1 == 0 || sqrMagn2 == 0)return 0;
	return dot / (sqrt(sqrMagn1) * sqrt(sqrMagn2));
}

/*		��������
	,|jk;p�������Ŷi�İ�����2389yfs���ߴ�ƽfopwiuyagifjdslakj iu�����޶���iŶ
	,|jk;p������Ŷi�İ���2389yfs����fopwiuyagifjdslakj iu��������iŶ
	a,b c d,ui
	ui,a,c,d
	  aabb,fff,�й�(��o��)��
		  aabb,fff,�й�(��o��)��
	3278hfjkali���з��ʰ��������oiu���߷��������۶����ղ�ͬŶiu������v����sent��������ytrscvtwert
	3278hfjkali���з��ʰ��������oiu��ż�������ܴӚGu��Ӵ����uv�����߶���ŶŶ��������ȥ��Ŷi�� iŷ����������iŶ���ַ�
	�õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv����
	�õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv����||
	�õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv����]]]]
	�õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv���󡿡���
	�������õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv����
	[[[�õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv����
	aaa�õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv����
	�õģ�uiofda����34rewq �ӿ��ٶ�������uŷ�ɲ�i��Ŷ���ķ��ơ�������i����˵�ڲ�i����Ҫv����aaa
	u����డ���������������͹�ˮ���uiorfgauisoeyruawgavoicfvweacgiuoeaw9809432��UFO i������ƤŶ�������ǹ�҅u���߶��㳡��uŶ�۶�
	u����డ���������������ͺ�ˮ���uiorfgauisoeyruawkavoicffweacgiuoeaw9809432��UFO i������Ƥ�Ұ������ǹ�҅u���߶��㳡��uŶ�۶�
*/

void Test()
{
	string s1 = "����Ϊ kllweŶ λ������";
	string s2 = "��ŶΪ kllwe λ�ڰ�����";
	vector<char>v1, v2;
	for (auto c : s1)
	{
		v1.push_back(c);
	}
	for (auto c : s2)
	{
		v2.push_back(c);
	}
	cout << CaculateSimilarity(CaculateWordFrequency(v1), CaculateWordFrequency(v2)) << endl;
}

int main(int argc, char* argv[])
{
	//Test();
	string orgPath, othPath, outPath;
	if (argc != 4)				//��cmd�л�ȡ����
	{
		cout << "�����������" << endl;
		return -1;	
	}

	orgPath = argv[1];		//�����ԭ���ĵľ��Ե�ַ
	othPath = argv[2];		//������������ĵľ��Ե�ַ
	outPath = argv[3];		//����ļ��ľ��Ե�ַ

	vector<char>res1, res2;														//��ȡ�����ַ�����
	if (ReadTxt(orgPath, res1) == false || ReadTxt(othPath, res2) == false)		//��ȡ�ļ�ʧ��
	{
		return -1;
	}

	//���ڴ�Ƶ�����������ƶ�
	double res = CaculateSimilarity(CaculateWordFrequency(res1), CaculateWordFrequency(res2));

	//�����д��ָ���ļ�
	fstream file;
	file.open(outPath, ios::out | ios::app);
	if (!file.is_open())			//�ļ���ʧ��
	{
		cout << "���ļ�" << outPath << "ʧ��" << endl;
		return -1;
	}
	//�ļ��б�����λС��
	file << setiosflags(ios::fixed) << setprecision(2) << res << endl;
	file.close();
	//����ֱ�����
	cout << "Similarity = " << res << endl;
	return 0;
}
