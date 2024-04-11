// #include <fstream>
// #include <sstream>
#include <iostream>
// #include <string>

#include <sstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>


struct Dat
{
	int x;
	int y;
	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(cereal::make_nvp("x", x), cereal::make_nvp("y", y));
	}
};


struct Data
{
	Dat dat = { 2,3 };
	int index = 1;
	double d[3] = { 1.0,2.33e-5,1000000.0 };
	std::vector<std::string> vs;
	char buf[10]="nihao\0";

	template <class Archive>
	void serialize(Archive & ar)
	{
		//加上_出现解析不到的问题 ar(cereal::make_nvp("_buf",std::string(buf)));
		ar(cereal::make_nvp("Dat", dat), cereal::make_nvp("index", index), cereal::make_nvp("d", d), cereal::make_nvp("vs", vs),cereal::make_nvp("buf",std::string(buf)));
	}
	
};

int main()
{
    Data mydata,mydata2;
	mydata.dat.x = 50;
	mydata.dat.y = 150;
	std::string s1 = "中文字符串/*-+!@#$%^&";
	std::string s2 = "english/*-+!@#$%^&";
	mydata.vs.push_back(s1);
	mydata.vs.push_back(s2);
	

	// std::ofstream file("out.json");
    // // std::string pp;
    std::ostringstream ss;
    
	cereal::JSONOutputArchive archive(ss);
	archive(CEREAL_NVP(mydata),CEREAL_NVP(mydata.index));
	// archive(cereal::make_nvp("mydata", mydata));
    std::cout<<ss.str()<<std::endl;


	/*
		不知道是特性，还是bug,使用fstring无误，使用ostringstream输出的字符串会缺少最后一个大括号,导致解析有误
	*/
	// std::ifstream file("out.json");
	std::string testStr=ss.str();//+" }";
	// std::cout<<testStr<<std::endl;
	std::istringstream iss(testStr);
	std::string buf;
	int index;
	
	try
	{
		cereal::JSONInputArchive archive1(iss);

		archive1(cereal::make_nvp("mydata", mydata2));
		archive1(cereal::make_nvp("mydata.index", index));
	}
	catch(const std::exception& e)
	{
		// cereal::RapidJSONException
		std::cerr << e.what() << '\n';
	}
	
	// archive1(cereal::make_nvp("mydata.buf", buf));
	std::cout<<mydata2.buf<<std::endl;
	// std::cout<<index<<std::endl;
}

// ------------------------------------------
// struct Data
// {
// 	int index = 1;
// 	double d[3] = { 1.0,2.33e-5,1000000.0 };
// 	std::vector<std::string> vs;

// 	template <class Archive>
// 	void serialize(Archive & ar)
// 	{
//         // ar(index, d, vs);
// 		ar(cereal::make_nvp("index", index), cereal::make_nvp("d", d), cereal::make_nvp("vs", vs));
// 	}
// };

// int main()
// {

// 	Data mydata;
// 	std::string s1 = "中文字符串/*-+!@#$%^&";
// 	std::string s2 = "english/*-+!@#$%^&";
// 	mydata.vs.push_back(s1);
// 	mydata.vs.push_back(s2);
// 	std::ofstream file("out1.json");

// 	cereal::JSONOutputArchive archive(file);
//     // archive(CEREAL_NVP(mydata.index), CEREAL_NVP(mydata.d), CEREAL_NVP(mydata.vs));
// 	archive(CEREAL_NVP(mydata));

// 	// std::ifstream file("out1.json");
// 	// cereal::JSONInputArchive archive(file);
// 	// int n;
// 	// std::vector<std::string> s;
// 	// double d[3];
//     // Data mydata;
// 	// // archive(cereal::make_nvp("mydata.index", n));
// 	// // archive(cereal::make_nvp("mydata.d", d));
// 	// // archive(cereal::make_nvp("mydata.vs", s));
//     // archive(cereal::make_nvp("mydata", mydata));
// 	// std::cout <<mydata.index << '\t'  << '\t' << mydata.d[1]; 

//     return 0;
// }

// ----------------------------------------------
    // std::ofstream file("out.json");
	// cereal::JSONOutputArchive archive(file);
	// std::string s[] = { "this is a string", " 中文string也是可以支持的" };
	// std::vector<double> vec = { 1.00001, 2e3, 30.1, -4, 5 };
	// archive(CEREAL_NVP(vec), CEREAL_NVP(s));

    // std::ifstream file("out.json");
	// cereal::JSONInputArchive archive(file);
	// std::string s[2];
	// archive(CEREAL_NVP(s));
	// std::cout << s[1];