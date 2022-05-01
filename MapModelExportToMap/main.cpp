#include "main.h"
#include <string>
#include <ctime>


int main()
{

	std::string in_war3_path = (fs::current_path().string() +"\\in.w3x");
	std::string root = fs::current_path().string();
	std::string res = root + "\\map\\resource\\";
	std::string map = root + "\\map\\map\\";
	std::string doo = map + "war3mapUnits.doo";
	std::string w3x2lni = root + "\\w3x2lni";
	int max_num = 400; //1200

	std::time_t t = std::time(0);  // t is an integer type
	std::cout << "Current working directory: " << fs::current_path() << '\n';
	std::string name = "SYGAME";
	printf("\n把你想要导成资源地图的改名为in.w3x即可点击运行\n");
	printf("BY:十月\n正在运行中....\n");
	try
	{
		fs::remove_all(res);
		std::filesystem::directory_iterator itr(root);
		while (itr != std::filesystem::directory_iterator()) {
			// 判断该目录是文件夹，跳过
			if (std::filesystem::is_directory(itr->path())) {
				itr++;
				continue;
			}
			std::string filePath = itr->path().string();
			std::string stem = itr->path().stem().string();        // 文件名
			std::string exten = itr->path().extension().string();   // 扩展名
			if (stem.find("map") == 0)
			{
				fs::remove(itr->path().string());
			}

			++itr;
		}
	}
	catch (const std::exception&)
	{
		printf("map.w3x文件已被打开，占用，不存在");
		printf("\n");
		system("pause");
		return 0;
	}
	try
	{
		std::filesystem::directory_iterator itr(map);
		while (itr != std::filesystem::directory_iterator()) {
			// 判断该目录是文件夹，跳过
			if (std::filesystem::is_directory(itr->path())) {
				itr++;
				continue;
			}
			std::string filePath = itr->path().string();
			std::string stem = itr->path().stem().string();        // 文件名
			std::string exten = itr->path().extension().string();   // 扩展名
			if (stem.find("war3mapUnits") == 0)
			{
				fs::remove(itr->path().string());
			}

			++itr;
		}
	}
	catch (const std::exception&)
	{
		printf("map文件夹不存在");
		printf("\n");
		system("pause");
		return 0;
	}
	/*system("pause");
	return 0;*/
	storm_mpq mpq;
	if (!mpq.Open(in_war3_path.c_str()))
	{
		printf("\n");
		system("pause");
		return 0;
	}
	std::string FileList[8192];
	int len = mpq.GetAllFileList(FileList, sizeof(FileList), "mdx");
	if (len <= 0)
	{
		printf("\n无可用资源\n");
		system("pause");
		return 0;
	}
	char nmae_list[8192][5];
	for (size_t i = 0; i < len; i++)
	{
		std::vector<fs::path> list;

		try
		{
			sprintf(nmae_list[i], "n%03X", i);
			if (mpq.ExportFile(FileList[i], res + name + "_" + std::to_string(t) + "_" + nmae_list[i] + ".mdx", &list, 0))
			{
				//printf("复制模型 %s \n", FileList[i].c_str());
			}


			//解析贴图
			for (auto& texture_path : list)
			{
				fs::path texture_temp = res / texture_path;
				if (mpq.ExportFile(texture_path, texture_temp, nullptr, 0))
				{
					//printf("复制贴图 %s \n", texture_path.string().c_str());
				}
			}
		}
		catch (const std::exception&)
		{
			printf("复制模型 %s 失败 \n", FileList[i].c_str());
		}
	}
	printf("全部模型文件导出成功！！共%d个", len);


	int cut_num = float(len / max_num + 1);
	//len = min(max_num, len);
	printf("\n=地图分割次数==%d==\n", cut_num);

	for (size_t cut_n = 1; cut_n <= cut_num; cut_n++)
	{
		auto* file = fopen((doo + std::to_string(cut_n)).c_str(), "wb");
		//头数据
		BYTE b[] = { 0x57,0x33,0x64, 0x6F, 0x08, 0x00, 0x00 ,0x00 ,0x0B ,00 ,00 ,00 };
		fwrite(b, 1, sizeof(b), file);
		int m_len = min(max_num, len - (cut_n - 1) * max_num);
		printf("\n=第%d次分割数量：=%d==\n", cut_n, m_len);
		//放置单位数量
		int num[1] = { m_len };
		fwrite(num, 1, sizeof(num), file);
		int x = 0;
		int y = 0;
		for (size_t i = 0; i < m_len; i++)
		{
			byte variation[3] = { };
			float Position_X[1] = { (500 * x + 1000) - 5800 };
			float Position_Y[1] = { (500 * y - 1000) - 3800 };
			byte data1[] = { 0x00,0x00,0x00,0x00,0x95,0xF4,0x3C,0x40,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F };
			byte data2[] = { 0x00,0x00,0x80,0x3F,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF };
			byte data3[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xD4,0x30,0x00,0x00,0x00 };
			byte data4[] = { 0x00,0x80,0xBF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
			byte data5[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
			byte data6[] = { 0x00,0x00,0x00,0xD0,0x94,0x65,0x7A,0x18,0x00,0x00,0x00,0x00,0x00,0x88,0x1E };

			fwrite(nmae_list[i + max_num * (cut_n - 1)], 1, sizeof(nmae_list[i + max_num * (cut_n - 1)]), file);
			fwrite(variation, 1, sizeof(variation), file);
			fwrite(Position_X, 1, sizeof(Position_X), file);
			fwrite(Position_Y, 1, sizeof(Position_Y), file);
			fwrite(data1, 1, sizeof(data1), file);
			fwrite(data2, 1, sizeof(data2), file);
			fwrite(data3, 1, sizeof(data3), file);
			fwrite(data4, 1, sizeof(data4), file);
			fwrite(data5, 1, sizeof(data5), file);
			fwrite(data6, 1, sizeof(data6), file);
			//std::cout << i + max_num * (cut_n - 1) << "-";
			if (i > 0 && x >= 19)
			{
				y++;
				x = 0;
			}
			else
			{
				x++;
			}
		}

		fclose(file);
	}

	printf("\n");
	system((w3x2lni + "\\bin\\w3x2lni-lua.exe " + w3x2lni + "\\run.lua " + root + " " + std::to_string(cut_num)).c_str());
	printf("\n");
	system("pause");
	return 0;
}
