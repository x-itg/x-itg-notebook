// 构造 vectors
#include <iostream>
#include <vector>
#include <iterator>
#include <deque>
using namespace std;

int main()
{
    deque<int> data = {10, 20, 30, 40};
    data.push_front(100);
    deque<int>::iterator dit;
    for (dit = data.begin(); dit != data.end(); dit++)
    {
        cout << *dit << " ";
    }
    cout << endl;

    data.pop_front();
    for (dit = data.begin(); dit != data.end(); dit++)
    {
        cout << *dit << " ";
    }
    cout << endl;

    vector<int> cond = {1, 2, 3, 3, 3, 3, 4, 5, 6};
    vector<int>::iterator it;
    for (it = cond.begin(); it != cond.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    for (it = cond.begin(); it != cond.end();)
    {
        if (*it == 3)
        {
            it = cond.erase(it); // 避免迭代器失效
        }
        else
        {
            it++;
        }
    }
    for (it = cond.begin(); it != cond.end(); it++)
    {
        cout << *it;
    }
    cout << endl;

    return 0;
}

/*
#include <boost/version.hpp>
#include <boost/config.hpp>
#include <boost/smart_ptr.hpp>
int main()
{
    // vector示例--------------------------------------

    std::cout << "-------vector示例-------" << std::endl;
    //  创建一个空的vector
    std::vector<int> v;

    // 向vector中添加元素
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    // 访问vector中的元素
    for (int i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << std::endl;
    }

    // 删除vector中的元素
    v.erase(v.begin());

    // 清空vector
    v.clear();
    // pair示例---------------------------------------------
    std::cout << "-------pair示例-------" << std::endl;
    std::cout << "两个不同的数据类型。它通常用于存储键值对" << std::endl;

    //  创建一个空的pair
    std::pair<int, std::string> p;

    // 向pair中添加元素
    p.first = 10;
    p.second = "Hello, world!";

    // 访问pair中的元素
    std::cout << p.first << std::endl;
    std::cout << p.second << std::endl;

    // vectoe结合pair示例---------------------------------------------
    std::cout << "-------vectoe结合pair示例-------" << std::endl;
    // 创建一个std::vector
    std::vector<std::pair<int, std::string>> pairs;

    // 向std::vector中添加元素
    pairs.push_back(std::make_pair(1, "Apple"));
    pairs.push_back(std::make_pair(2, "Orange"));
    pairs.push_back(std::make_pair(3, "Banana"));

    // 遍历std::vector中的所有元素
    for (int i = 0; i < pairs.size(); i++)
    {
        std::cout << pairs[i].first << " " << pairs[i].second << std::endl;
    }
    // boost示例----------------------------------------------------
    std::cout << "-------boost示例-------" << std::endl;
    std::cout << "中文语言环境" << std::endl;
    std::cout << "Boost Lib 版本:" << BOOST_LIB_VERSION << std::endl;
    std::cout << "当前平台:" << BOOST_PLATFORM << std::endl;
    std::cout << "编译器:" << BOOST_COMPILER << std::endl;
    std::cout << "STL 库版本" << BOOST_STDLIB << std::endl;
    boost::shared_ptr<std::string> msg;
    msg.reset(new std::string("文字消息----"));
    std::cout << msg << "--" << msg.get() << "--" << &msg << "--" << msg->data() << std::endl;
    msg.reset();

    return 0;
}
*/