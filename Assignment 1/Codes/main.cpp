#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
class BMPImage{
public:
  BMPImage(std::vector<unsigned char>* _buffer);
  void filter1();
  void filter2();
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
    imageWidth += (buffer->operator[](i+18)<<(i*8));
    imageHeight += (buffer->operator[](i+22)<<(i*8));
    byteOffset += (buffer->operator[](i+10)<<(i*8));
  }
}
void BMPImage::filter1(){
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
void BMPImage::filter2(){
unsigned char tempPixel1;
unsigned char tempPixel2;
unsigned char tempPixel3;
  for(int i=0; i<imageHeight; i++){
    for(int j=0; j<(imageWidth/2); j++){
      tempPixel1 = buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*(imageWidth-j-1));
      tempPixel2 = buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*(imageWidth-j-1)+1);
      tempPixel3 = buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*(imageWidth-j-1)+2);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*(imageWidth-j-1)) = buffer->operator[](54 + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*(imageWidth-j-1)+1) = buffer->operator[](54 + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+1);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*(imageWidth-j-1)+2) = buffer->operator[](54 + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+2);
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j) = tempPixel1;
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+1) = tempPixel2;
      buffer->operator[](byteOffset + i*(3*imageWidth+(4-imageWidth%4)%4)+3*j+2) = tempPixel3;
    }
  }
}
void BMPImage::writeImage(){
  std::ofstream outFile("out.bmp");
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
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    BMPImage bmp(&buffer);
    bmp.filter1();
    bmp.writeImage();
  }
  else{
    std::cout << "Image has not been find." << std::endl;
    return 1;
  }
  return 0;
}
