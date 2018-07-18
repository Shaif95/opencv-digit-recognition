#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using namespace cv;

#include "mousedraw.h"


#define resulttxt "D:/project/HOG/t10k-images-bmp/t10k-images/result.txt"

void buildImg()
{
    BoxExtractor box;
    Mat res = box.MouseDraw ("draw",Mat(400,400,CV_8UC3,Scalar(0,0,0)),Scalar(255,255,255),18);
    imwrite ("d:\\test.jpg",res);
}

void testTrain()
{
    vector<string> img_path;//输入文件名变量
    vector<int> img_catg;
    int nLine = 0;
    string buf;
    ifstream svm_data(resulttxt);//训练样本图片的路径都写在这个txt文件中，使用bat批处理文件可以得到这个txt文件
    unsigned long n;
    while( svm_data )//将训练样本文件依次读取进来
    {
        if( getline( svm_data, buf ) )
        {
            nLine ++;
            if( nLine % 2 == 0 )//注：奇数行是图片全路径，偶数行是标签
            {
                img_catg.push_back( atoi( buf.c_str() ) );//atoi将字符串转换成整型，标志(0,1，2，...，9)，注意这里至少要有两个类别，否则会出错
            }
            else
            {
                img_path.push_back( buf );//图像路径
            }
        }
    }
    svm_data.close();//关闭文件
    CvMat *data_mat, *res_mat;
    int nImgNum = nLine / 2; //nImgNum是样本数量，只有文本行数的一半，另一半是标签
    data_mat = cvCreateMat( nImgNum, 324, CV_32FC1 );  //第二个参数，即矩阵的列是由下面的descriptors的大小决定的，可以由descriptors.size()得到，且对于不同大小的输入训练图片，这个值是不同的
    cvSetZero( data_mat );
    //类型矩阵,存储每个样本的类型标志
    res_mat = cvCreateMat( nImgNum, 1, CV_32FC1 );
    cvSetZero( res_mat );
    IplImage* src;
    IplImage* trainImg=cvCreateImage(cvSize(28,28),8,3);//需要分析的图片，这里默认设定图片是28*28大小，所以上面定义了324，如果要更改图片大小，可以先用debug查看一下descriptors是多少，然后设定好再运行
    //处理HOG特征
    for( string::size_type i = 0; i != img_path.size(); i++ )
    {
        src=cvLoadImage(img_path[i].c_str(),1);
        if( src == NULL )
        {
            cout<<" can not load the image: "<<img_path[i].c_str()<<endl;
            continue;
        }
        cout<<"deal with\t"<<img_path[i].c_str()<<endl;
        cvResize(src,trainImg);
        HOGDescriptor *hog=new HOGDescriptor(cvSize(28,28),cvSize(14,14),cvSize(7,7),cvSize(7,7),9);
        vector<float>descriptors;//存放结果
        hog->compute(trainImg, descriptors,Size(1,1), Size(0,0)); //Hog特征计算
        cout<<"HOG dims: "<<descriptors.size()<<endl;
        n=0;
        for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)
        {
            cvmSet(data_mat,i,n,*iter);//存储HOG特征
            n++;
        }
        cvmSet( res_mat, i, 0, img_catg[i] );
        cout<<"Done !!!: "<<img_path[i].c_str()<<" "<<img_catg[i]<<endl;
    }
    CvSVM svm;//新建一个SVM
    CvSVMParams param;//这里是SVM训练相关参数
    CvTermCriteria criteria;
    criteria = cvTermCriteria( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );
    param = CvSVMParams( CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria );
    svm.train( data_mat, res_mat, NULL, NULL, param );//训练数据
    //保存训练好的分类器
    std::cout<<"saving... ... !!! \n "<<endl;
    svm.save( "d:\\HOG_SVM_DATA.xml" );
    cout<<"HOG_SVM_DATA.xml is saved !!! \n exit train process"<<endl;
    cvReleaseMat( &data_mat );
    cvReleaseMat( &res_mat );
    cvReleaseImage(&trainImg);
}

void testPredict(){
    IplImage *test;
    char result[300]; //存放预测结果

    CvSVM svm;
    svm.load("d:\\HOG_SVM_DATA.xml");//加载训练好的xml文件，这里训练的是10K个手写数字
    //检测样本
    test = cvLoadImage("d:\\test.jpg", 1); //待预测图片，用系统自带的画图工具随便手写
    if (!test)
    {
        cout<<"not exist"<<endl;
        return ;
    }
    cout<<"load image done"<<endl;
    IplImage* trainTempImg=cvCreateImage(cvSize(28,28),8,3);
    cvZero(trainTempImg);
    cvResize(test,trainTempImg);
    HOGDescriptor *hog=new HOGDescriptor(cvSize(28,28),cvSize(14,14),cvSize(7,7),cvSize(7,7),9);
    vector<float>descriptors;//存放结果
    hog->compute(trainTempImg, descriptors,Size(1,1), Size(0,0)); //Hog特征计算
    cout<<"HOG dims: "<<descriptors.size()<<endl;  //打印Hog特征维数  ，这里是324
    CvMat* SVMtrainMat=cvCreateMat(1,descriptors.size(),CV_32FC1);
    int n=0;
    for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)
    {
        cvmSet(SVMtrainMat,0,n,*iter);
        n++;
    }

    int ret = svm.predict(SVMtrainMat);//检测结果
    sprintf(result, "%d\r\n",ret );
    cvNamedWindow("test.jpg",1);
    cvShowImage("test.jpg",test);
    cout<<"predict result:"<<result<<endl;
    waitKey ();
    cvReleaseImage(&test);
    cvReleaseImage(&trainTempImg);
}

int main()
{
    string input = "";
    int myNumber = 0;
    while (true) {
        cout << "Please Select: \n1  build the d:\\test.jpg\n2  build the d:\\HOG_SVM_DATA.xml\n3  predict the d:\\test.jpg\n\n";
        getline(cin, input);
        stringstream myStream(input);
        if (myStream >> myNumber)
            break;
        cout << "Invalid number, please try again" << endl;
    }
    cout << "You entered: " << myNumber << endl << endl;
    switch( myNumber ){
    // update the selected bounding box
    case 1:
        buildImg(); // 1. build the d:\\test.jpg
        break;
    case 2:
        testTrain();   // 2. build the d:\\HOG_SVM_DATA.xml
        break;
    case 3:
        testPredict();  // 3. predict the d:\\test.jpg
        break;
    }
    cout << endl;
    return 0;
}





