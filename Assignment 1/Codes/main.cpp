// Links to be read:
// 1-https://courses.cs.vt.edu/~cs2604/fall00/binio.html
// 2-https://www.gnu.org/software/libc/manual/html_node/Binary-Streams.html
// 3-https://stackoverflow.com/questions/10748506/what-does-it-mean-to-bitwise-left-shift-an-unsigned-char-with-16
// Add this method of reading from the files:
// ifile.open(file.c_str(), std::ios::binary);
// std::vector<uint32_t> datavec;
//
// if (ifile.is_open())
// {
//     ifile.seekg(0,ifile.end);
//     int flen = ifile.tellg();
//     ifile.seekg(0,ifile.beg);
//     datavec.resize(flen/sizeof(uint32_t));
//     ifile.read((char*)&datavec.front(), datavec.size()*sizeof(uint32_t));
//     ifile.close();
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
class BMPImage{
public:
  BMPImage(std::vector<unsigned char>* _buffer);
  void mirrorFilter();
  void blackAndWhiteFilter();
  void rotationFilter();
  void writeImage();
  int byteOffset;
private:
  std::vector<unsigned char>* buffer;
  int imageWidth;
  int imageHeight;
  int fileSize;
};
BMPImage::BMPImage(std::vector<unsigned char>* _buffer){
  buffer = _buffer;
  fileSize = buffer->size();
  imageWidth = 0;
  imageHeight = 0;
  byteOffset = 0;
  for(int i=0; i<4; i++){
//** good reference: https://stackoverflow.com/questions/10748506/what-does-it-mean-to-bitwise-left-shift-an-unsigned-char-with-16
    imageWidth += (buffer->operator[](i+18)<<(i*8));
    imageHeight += (buffer->operator[](i+22)<<(i*8));
    byteOffset += (buffer->operator[](i+10)<<(i*8));
  }
}
void BMPImage::blackAndWhiteFilter(){
unsigned int tempPixel1;
unsigned int tempPixel2;
unsigned int tempPixel3;
  for(int i=0; i<imageHeight; i++){
    for(int j=0; j<imageWidth; j++){
      tempPixel1 = (unsigned int)buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j);
      tempPixel2 = (unsigned int)buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+1);
      tempPixel3 = (unsigned int)buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+2);
      // I don't know why, but if we cast unsigned int to unsigned char and then put it into our buffer, our algorithm won't work.
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j) =  (tempPixel1+tempPixel2+tempPixel3)/3;
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+1) =  (tempPixel1+tempPixel2+tempPixel3)/3;
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+2) =  (tempPixel1+tempPixel2+tempPixel3)/3;
    }
  }
}
void BMPImage::mirrorFilter(){
unsigned char tempPixel1;
unsigned char tempPixel2;
unsigned char tempPixel3;
  for(int i=0; i<imageHeight; i++){
    for(int j=0; j<(imageWidth/2); j++){
      tempPixel1 = buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*(imageWidth-j-1));
      tempPixel2 = buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*(imageWidth-j-1)+1);
      tempPixel3 = buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*(imageWidth-j-1)+2);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*(imageWidth-j-1)) = buffer->operator[](54 + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*(imageWidth-j-1)+1) = buffer->operator[](54 + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j+1);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*(imageWidth-j-1)+2) = buffer->operator[](54 + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j+2);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j) = tempPixel1;
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j+1) = tempPixel2;
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j+2) = tempPixel3;
    }
  }
}
void BMPImage::rotationFilter(){
std::vector<std::vector<unsigned char>> buffer_temp(imageHeight, std::vector<unsigned char>(3*imageWidth, 0));
unsigned char imageDimensionTemp;
for(int i=0; i<imageHeight; i++){
  for(int j=0; j<imageWidth; j++){
    buffer_temp[i][3*j] = buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j);
    buffer_temp[i][3*j+1] = buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j+1);
    buffer_temp[i][3*j+2] = buffer->operator[](byteOffset + i*(3*imageWidth+(4-3*imageWidth%4)%4)+3*j+2);
  }
}
for(int i=0; i<4; i++){
  imageDimensionTemp = buffer->operator[](18+i);
  buffer->operator[](18+i) = buffer->operator[](22+i);
  buffer->operator[](22+i) = imageDimensionTemp;
}
for(int i=0; i<imageWidth; i++){
  for(int j=0; j<imageHeight; j++){
    buffer->operator[](byteOffset + i*(3*imageHeight+(4-3*imageHeight%4)%4)+3*j) = buffer_temp[j][3*(imageWidth-1)-3*i];
    buffer->operator[](byteOffset + i*(3*imageHeight+(4-3*imageHeight%4)%4)+3*j+1) = buffer_temp[j][3*(imageWidth-1)-3*i+1];
    buffer->operator[](byteOffset + i*(3*imageHeight+(4-3*imageHeight%4)%4)+3*j+2) = buffer_temp[j][3*(imageWidth-1)-3*i+2];
  }
  for(int k=0; k<((4-3*imageHeight%4)%4); k++){
    buffer->operator[](byteOffset + i*(3*imageHeight+(4-3*imageHeight%4)%4)+3*imageHeight+k) = 0;
  }
}
int temp_imageHeight = imageHeight;
imageHeight = imageWidth;
imageWidth = imageHeight;
}




void BMPImage::writeImage(){
  std::ofstream outFile("out.bmp");
//** Instead of for loop you can use write method.
//** Reference on writeing in files using write method: https://www.cplusplus.com/reference/ostream/ostream/write/
  for (int i=0; i<fileSize; i++){
    outFile << (buffer->operator[](i));
    // outFile << (*buffer)[i];
  }
  outFile.close();
}
int main(int argc, char *argv[]){
  if(argc<2){
    std::cout << "Please provide an input." << std::endl;
    return 1;
  }
  std::ifstream inFile(argv[1]);
  if(inFile){
// Reference: https://stackoverflow.com/questions/27406789/confused-about-usage-of-stdistreambuf-iterator
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    BMPImage bmp(&buffer);
    // bmp.blackAndWhiteFilter();

    try{
      po::options_description desc{"Program Options"};
      desc.add_options()
        ("help,h", "Help Screen")
        ("Filter_1", "Black and Whith Filter")
        ("Filter_2", "Mirror Filter")
        ("Filter_3", "Rotation Filter");
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, desc), vm);
      if (vm.count("help"))
        std::cout << desc << std::endl;
      if(vm.count("Filter_1")){
        bmp.blackAndWhiteFilter();
      }
      if(vm.count("Filter_2")){
        bmp.mirrorFilter();
      }
      if(vm.count("Filter_3")){
        bmp.rotationFilter();
      }
    }catch (const po::error &ex){
      std::cerr << ex.what() << '\n';
    }
    bmp.writeImage();
  }
  else{
    std::cout << "Image has not been find." << std::endl;
    return 1;
  }










  return 0;
}
