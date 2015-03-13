#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ColorHistogram {
private:
	int histSize[3];
	float hranges[2];
	const float* ranges[3];
	int channels[3];
public:
	ColorHistogram() {
		// 컬러 히스토그램을 위한 인자 준비
		histSize[0]= histSize[1]= histSize[2]= 256;
		hranges[0]= 0.0;    // BRG 범위
		hranges[1]= 255.0;
		ranges[0]= hranges; // 모든 채널은 같은 범위를 가짐 
		ranges[1]= hranges; 
		ranges[2]= hranges; 
		channels[0]= 0;  // 세 가지 채널 
		channels[1]= 1; 
		channels[2]= 2; 
	}

	// 히스토그램 계산.
	cv::MatND getHistogram(const cv::Mat &image) {

		cv::MatND hist;

		// BGR 컬러 히스토그램
		hranges[0]= 0.0;    // BRG 범위
		hranges[1]= 255.0;
		channels[0]= 0;  // 세 가지 채널
		channels[1]= 1; 
		channels[2]= 2; 

		// 히스토그램 계산
		cv::calcHist(&image, 
			1,   // 단일 영상의 히스토그램만
			channels, // 대상 채널
			cv::Mat(), //마스크 사용하지 않음
			hist,  // 결과 히스토그램
			3,   // 3D 히스토그램
			histSize, // 빈도수
			ranges  // 화소값 범위
			);

		return hist;
	}

	// 히스토그램 계산.
	cv::SparseMat getSparseHistogram(const cv::Mat &image) {

		cv::SparseMat hist(3,histSize,CV_32F);

		// BGR 컬러 히스토그램
		hranges[0]= 0.0;    // BRG 범위
		hranges[1]= 255.0;
		channels[0]= 0;  // 세 가지 채널 
		channels[1]= 1; 
		channels[2]= 2; 

		// Compute histogram
		cv::calcHist(&image, 
			1,   // 단일 영상의 히스토그램만
			channels, // 대상 채널
			cv::Mat(), // 마스크 사용하지 않음      
			hist,  // 결과 히스토그램   
			3,   // 3D 히스토그램          
			histSize, // 빈도수                 
			ranges  // 화소값 범위            
			);

		return hist;
	}

	cv::Mat colorReduce(const cv::Mat &image, int div=64) {

		int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
		// 화소값 반올림 하기 위해 사용하는 마스크
		uchar mask= 0xFF<<n; // 예, div=16이면 mask= 0xF0

		cv::Mat_<cv::Vec3b>::const_iterator it= image.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator itend= image.end<cv::Vec3b>();

		// 결과 영상 설정(항상 1-채널)
		cv::Mat result(image.rows,image.cols,image.type());
		cv::Mat_<cv::Vec3b>::iterator itr= result.begin<cv::Vec3b>();

		for ( ; it!= itend; ++it, ++itr) {

			(*itr)[0]= ((*it)[0]&mask) + div/2;
			(*itr)[1]= ((*it)[1]&mask) + div/2;
			(*itr)[2]= ((*it)[2]&mask) + div/2;
		}

		return result;
	}
};





class ImageComparator {
private:
	cv::Mat reference; // 원본
	cv::Mat input; // 비교할 대상
	cv::MatND refH;
	cv::MatND inputH;

	ColorHistogram hist;
	int div;

public:
	ImageComparator() : div(32) { // div=32 초기화
	}

	// 신뢰할 수 있는 유사도 측정 방법을 얻으려면 컬러 개수를 반드시 감축해야 함
	// 클래스에 컬러 감축 인자를 포함하며, 질의와 입력 영상에 모두 적용
	void setColorReduction( int factor) {
		// 컬러 감축 인자
		// 각 차원별 인자에 의해 컬러 공간 감축 시킨 영상 비교
		div= factor;
	}

	int getColorReduction() {
		return div;
	}

	void setReferenceImage(const cv::Mat& image) {
		// 질의 영상도 적절한 세터를 사용해 지정, 질의 영상에 대해 컬러를 감축
		reference= hist.colorReduce(image,div);
		refH= hist.getHistogram(reference);
	}

	double compare(const cv::Mat& image) {
		// compare 메소드는 지정한 입력 영상과 참조 영상을 비교
		// 두 영상이 얼마나 유사한지 알려주는 점수 반환
		input= hist.colorReduce(image,div);
		inputH= hist.getHistogram(input);

		return cv::compareHist(refH,inputH,CV_COMP_BHATTACHARYYA);
	}
};