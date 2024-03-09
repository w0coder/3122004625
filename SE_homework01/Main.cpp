#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>

using namespace std;

//读取文件到 res 中
bool ReadTxt(const string& path, vector<char>& res)
{
	fstream file;
	file.open(path, ios::in);
	if (!file.is_open())
	{
		cout << "读取文件" << path << "失败" << endl;
		return false;
	}
	vector<char> buf(file.seekg(0, ios::end).tellg());
	file.seekg(0, ios::beg).read(&buf[0], static_cast<streamsize>(buf.size()));

	file.close();
	res = buf;

	return true;
}

//统计词频 （UTF-8编码形式）
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
		if (resMap.find(word) != resMap.end())resMap[word]++;	//表中有该词
		else resMap[word] = 1;									//表中没有
		i += len;
		word.clear();
	}
	/*
	for (int i = 0; i < txt.size();)
	{
		if ((txt[i] & 0x80) && word.empty())			//中文统计一个字（gbk编码）
		{
			word.push_back(txt[i]);
			word.push_back(txt[i + 1]);
			i += 2;
		}
		else if ((txt[i] & 0x80)==0)					//英文统计一个单词
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

//计算余弦相似度
double CaculateSimilarity(const unordered_map<string, int>& freq1, const unordered_map<string, int>& freq2)
{
	double dot = 0, sqrMagn1 = 0, sqrMagn2 = 0;
	for (auto& pair : freq1)
	{
		if (freq2.find(pair.first) != freq2.end())			//在另一篇中也有这个词
			dot += pair.second * freq2.at(pair.first);
		sqrMagn1 += pair.second * pair.second;				//第一篇的词频的平方和
	}
	for (auto& pair : freq2)
	{
		sqrMagn2 += pair.second * pair.second;				//第二篇的词频的平方和
	}
	if (sqrMagn1 == 0 || sqrMagn2 == 0)return 0;
	return dot / (sqrt(sqrMagn1) * sqrt(sqrMagn2));
}

/*		测试用例
	,|jk;p工会费哇哦i文案噢批2389yfs【高代平fopwiuyagifjdslakj iu噢批娃儿给i哦
	,|jk;p工会哇哦i文案批2389yfs【高fopwiuyagifjdslakj iu噢批儿给i哦
	a,b c d,ui
	ui,a,c,d
	  aabb,fff,中国(⊙o⊙)？
		  aabb,fff,中国(⊙o⊙)？
	3278hfjkali呼叫访问案例；快捷oiu费瓦夫擦测如此哇饿啊日不同哦iu人特少v让他sent与他人是ytrscvtwert
	3278hfjkali呼叫访问案例；快捷oiu不偶尔外语能从欸u哇哟人体uv他们瓦尔【哦哦亲重新我去饿哦i你 i欧福湾鳄参与i哦二分法
	好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕
	好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕||
	好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕]]]]
	好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕】】】
	【【【好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕
	[[[好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕
	aaa好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕
	好的，uiofda】、34rewq 加快速度拉法基u欧派不i哇哦拍拍佛牌【饿啊哇i阿婆说内测i啊不要v铁腕aaa
	u翻译多啊不过护卫舰饿啊巴哈水电费uiorfgauisoeyruawgavoicfvweacgiuoeaw9809432】UFO i撒负担皮哦啊二、呵国覅u破瓦尔广场就u哦哇饿
	u翻多多啊进过护卫环饿啊巴和水电费uiorfgauisoeyruawkavoicffweacgiuoeaw9809432】UFO i撒负担皮我啊二、呵国覅u破瓦尔广场就u哦哇饿
*/

void Test()
{
	string s1 = "发于为 kllwe哦 位埃——";
	string s2 = "发哦为 kllwe 位于埃——";
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
	if (argc != 4)				//从cmd中获取参数
	{
		cout << "输入参数错误" << endl;
		return -1;	
	}

	orgPath = argv[1];		//输入的原论文的绝对地址
	othPath = argv[2];		//输入的其他论文的绝对地址
	outPath = argv[3];		//输出文件的绝对地址

	vector<char>res1, res2;														//读取到的字符数组
	if (ReadTxt(orgPath, res1) == false || ReadTxt(othPath, res2) == false)		//读取文件失败
	{
		return -1;
	}

	//基于词频计算余弦相似度
	double res = CaculateSimilarity(CaculateWordFrequency(res1), CaculateWordFrequency(res2));

	//将结果写入指定文件
	fstream file;
	file.open(outPath, ios::out | ios::app);
	if (!file.is_open())			//文件打开失败
	{
		cout << "打开文件" << outPath << "失败" << endl;
		return -1;
	}
	//文件中保留两位小数
	file << setiosflags(ios::fixed) << setprecision(2) << res << endl;
	file.close();
	//窗口直接输出
	cout << "Similarity = " << res << endl;
	return 0;
}
