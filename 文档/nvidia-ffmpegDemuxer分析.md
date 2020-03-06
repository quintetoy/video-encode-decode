# nvidia-ffmpegDemuxer分析

包含检查函数，常用ck()来检验，在源代码中常见

```cpp
./Utils/NvCodecUtils.h://! \brief Miscellaneous classes and error checking functions.
./Utils/NvCodecUtils.h:inline bool check(CUresult e, int iLine, const char *szFile) {
./Utils/NvCodecUtils.h:inline bool check(cudaError_t e, int iLine, const char *szFile) {
./Utils/NvCodecUtils.h:inline bool check(NVENCSTATUS e, int iLine, const char *szFile) {
./Utils/NvCodecUtils.h:inline bool check(HRESULT e, int iLine, const char *szFile) {
./Utils/NvCodecUtils.h:inline bool check(GLenum e, int iLine, const char *szFile) {
./Utils/NvCodecUtils.h:inline bool check(int e, int iLine, const char *szFile) {
./Utils/NvCodecUtils.h:#define ck(call) check(call, __LINE__, __FILE__)
```



**另外函数使用了嵌套类**

**一、嵌套类**

 <https://www.cnblogs.com/lidabo/p/3772477.html>

   在一个类的内部定义另一个类，我们称之为嵌套类（nested class），或者嵌套类型。之所以引入这样一个嵌套类，往往是因为外围类需要使用嵌套类对象作为底层实现，并且该嵌套类只用于外围类的实现，且同时可以对用户隐藏该底层实现。


   **虽然嵌套类在外围类内部定义，但它是一个独立的类，基本上与外围类不相关**。它的成员不属于外围类，同样，外围类的成员也不属于该嵌套类。嵌套类的出现只是告诉外围类有一个这样的类型成员供外围类使用。并且，外围类对嵌套类成员的访问没有任何特权，嵌套类对外围类成员的访问也同样如此，它们都遵循普通类所具有的标号访问控制。

 

   若不在嵌套类内部定义其成员，则其定义只能写到与外围类相同的作用域中，且要用外围类进行限定，不能把定义写在外围类中。例如，嵌套类的静态成员就是这样的一个例子。


   前面说过，之所以使用嵌套类的另一个原因是达到底层实现隐藏的目的。为了实现这种目的，我们需要在另一个头文件中定义该嵌套类，而只在外围类中前向声明这个嵌套类即可。当然，在外围类外面定义这个嵌套类时，应该使用外围类进行限定。使用时，只需要在外围类的实现文件中包含这个头文件即可。

   另外，嵌套类可以直接引用外围类的静态成员、类型名和枚举成员（假定这些成员是公有的）。类型名是一个typedef名字、枚举类型名、或是一个类名。



**总结就是可以在外部类中使用嵌套的类，类似于包含了头文件，可以使用int，等等的基本类型。嵌套完以后，可以直接使用，转换类型等等**

```cpp
class FFmpegDemuxer{
public:
    class DataProvider {//共有的，外部可访问
    public:
        virtual ~DataProvider() {}
        virtual int GetData(uint8_t *pBuf, int nBuf) = 0;
    };
  
private:
  //此处ReadPacket是类中的成员函数，如果要访问，则ReadPacket只能为静态成员函数，它是一个回调函数
  AVFormatContext *CreateFormatContext(DataProvider *pDataProvider){
            avioc = avio_alloc_context(avioc_buffer, avioc_buffer_size,
            0, pDataProvider, &ReadPacket, NULL, NULL);
  }
  

  //所有对象共享的函数，只能访问静态成员
  //相当于一个实现函数
  //使用static的原因
  
  //没有对象的时候要访问类成员函数的话，只能访问静态成员函数。
      static int ReadPacket(void *opaque, uint8_t *pBuf, int nBuf) {
        return ((DataProvider *)opaque)->GetData(pBuf, nBuf);
    }
}
```





**static类成员**

<https://www.cnblogs.com/fuqia/p/8888938.html>

static修饰类中成员，表示类的共享数据

**1.static类成员**

C++primer里面说过，static类成员不像普通的类数据成员，static类数据成员独立于一切类对象处在。static类数据成员是与类关联的，

但不与该类定义的对象有任何关系。即static不会像普通类数据成员一样每一个类对象都有一份，全部类对象是共享一个static类成员的。

例如A类对象修改了static成员为1，那么B对象对应的static类对象成员的值也会是1。

**注意：static类对象必须要在类外进行初始化**

static修饰的变量先于对象存在，所以static修饰的变量要在类外初始化。因为static是所有对象共享的变量，必须要比对象先存在。

```cpp
class Text  
{  
    public:  
    static int count;  
};  
  
int Text::count=0;//用static成员变量必须要初始化   
  
int main()  
{  
    Text t1;  
    Text t2;  
      
    t1.count = 100;     //t1对象把static成员count改为100   
    cout<<t2.count<<endl; //当t2对象打印static成员的时候，显示的是100而不是0   
    return 0;  
}
```



**2.static类成员函数**

由于static修饰的类成员属于类，不属于对象，因此static类成员函数是没有this指针的，this指针是指向本对象的指针。正因为没有this指针，所以static类成员函数

不能访问非static的类成员，只能访问 static修饰的类成员。



**3.静态成员函数使用**

<https://www.cnblogs.com/virusolf/p/5556601.html>

　2).  使用静态成员函数，静态成员函数不使用this指针作为隐含参数，这样就可以作为回调函数了。静态成员函数具有两大特点：**其一，可以在没有类实例的情况下使用；**

**其二，只能访问静态成员变量和静态成员函数，不能访问非静态成员变量和非静态成员函数。**由于在C++中使用类成员函数作为回调函数的目的就是为了访问所有的成员变量和成员函数，如果做不到这一点将不具有实际意义。解决的办法也很简单，就是使用一个静态类指针作为类成员，通过在类创建时初始化该静态指针，如pThis=this，然后在回调函数中通过该静态指针就可以访问所有成员变量和成员函数了。这种处理办法适用于只有一个类实例的情况，因为多个类实例将共享静态类成员和静态成员函数，这就导致静态指针指向最后创建的类实例。为了避免这种情况，可以使用回调函数的一个参数来传递this指针，从而实现数据成员共享。这种方法稍稍麻烦，这里就不再赘述。





```cpp
class Test
{
public:

    static void callBackFun(void){}; //因为callBackFun默认有一个const Test* 的指针
};

//函数，需要调用回调函数
typedef void (*FPtr)(void);

void Fun(FPtr ptr)
{
    ptr();
}

void main(void)
{
    Fun(Test::callBackFun); //直接调用
}
```















