#include "tutorial.hpp"

#include <opencv2/core.hpp>

#include <iostream>
#include <string>


constexpr auto FILE_PATH = "D:/repos/OpenCV/Tutorial/out_files/read_write.xml"; // .yml/yaml/xml/yml.gz/xml.gz


class MyData
{
public:
    // Data Members
    int A;
    double X;
    std::string id;

    MyData() : A(0), X(0), id()
    {}


    explicit MyData(int) : A(97), X(CV_PI), id("mydata1234") // explicit to avoid implicit conversion
    {}


    void write(cv::FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "A" << A << "X" << X << "id" << id << "}";
    }


    void read(const cv::FileNode& node)                          //Read serialization for this class
    {
        A = (int)node["A"];
        X = (double)node["X"];
        id = (std::string)node["id"];
    }

};


//These write and read functions must be defined for the serialization in FileStorage to work
static void write(cv::FileStorage& fs, const std::string&, const MyData& x)
{
    x.write(fs);
}


static void read(const cv::FileNode& node, MyData& x, const MyData& default_value = MyData())
{
    if (node.empty())
        x = default_value;
    else
        x.read(node);
}


// This function will print our custom class to the console
static std::ostream& operator<<(std::ostream& out, const MyData& m)
{
    out << "{ id = " << m.id << ", ";
    out << "X = " << m.X << ", ";
    out << "A = " << m.A << "}";
    return out;
}


void write_data()
{
    cv::Mat R = cv::Mat_<uchar>::eye(3, 3),
        T = cv::Mat_<double>::zeros(3, 1);

    MyData m(1);

    cv::FileStorage fs(FILE_PATH, cv::FileStorage::WRITE);
    // or:
    // FileStorage fs;
    // fs.open(filename, FileStorage::WRITE);

    fs << "iterationNr" << 100;

    fs << "strings" << "[";                              // text - string sequence
    fs << "image1.jpg" << "Awesomeness" << "../data/baboon.jpg";
    fs << "]";                                           // close sequence

    fs << "Mapping";                              // text - mapping
    fs << "{" << "One" << 1;
    fs << "Two" << 2 << "}";

    fs << "R" << R;                                      // cv::Mat
    fs << "T" << T;

    fs << "MyData" << m;                                // your own data structures
    fs.release();                                       // explicit close
    std::cout << "Write Done.\n";
}


void read_data()
{
    std::cout << "\nReading:\n";

    cv::FileStorage fs;
    fs.open(FILE_PATH, cv::FileStorage::READ);

    int itNr;
    //fs["iterationNr"] >> itNr;
    itNr = (int)fs["iterationNr"];

    std::cout << itNr;
    if (!fs.isOpened())
    {
        std::cerr << "Failed to open " << FILE_PATH << '\n';
        return;
    }

    cv::FileNode n = fs["strings"];                         // Read string sequence - Get node
    if (n.type() != cv::FileNode::SEQ)
    {
        std::cerr << "strings is not a sequence! FAIL\n";
    }

    cv::FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
    for (; it != it_end; ++it)
        std::cout << (std::string) * it << '\n';

    n = fs["Mapping"];                                // Read mappings from a sequence
    std::cout << "Two  " << (int)(n["Two"]) << "; ";
    std::cout << "One  " << (int)(n["One"]) << "\n\n";

    MyData m;
    cv::Mat R, T;

    fs["R"] >> R;                                      // Read cv::Mat
    fs["T"] >> T;
    fs["MyData"] >> m;                                 // Read your own structure_

    std::cout << '\n'
        << "R = " << R << '\n';

    std::cout << "T = " << T << "\n\n";

    std::cout << "MyData = " << '\n' << m << "\n\n";

    //Show default behavior for non existing nodes
    std::cout << "Attempt to read NonExisting (should initialize the data structure with its default).";
    fs["NonExisting"] >> m;
    std::cout << '\n' << "NonExisting = " << '\n' << m << '\n';
}


void read_write()
{
    write_data();
    read_data();
}