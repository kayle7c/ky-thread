# V1.3

## 写在前面

​	KY-THREAD是仿照RT-THREAD实现的一个实时操作系统，在代码规范上大部分和RT-THREAD一致，文件目录也是参照RT-THREAD的文件目录，目前已经实现了支持多优先级和同优先级下的时间片轮转的线程调度，可以实现最基本的实时操作系统功能。在临界区和线程切换的汇编实现上暂时沿用了RT-THREAD的代码，在后面补充Cortex-M3的相关知识后再重构。

## 文件目录

```
|--ky-thread
   |__include	  内核头文件
   |__libcpu	  芯片移植代码
   	  |__Cotex-M3
   |__src		  内核源代码
   |__components  组件代码
```

​	在目前libcpu里只有Cortex-M3内核，暂时不支持其他内核。

## 功能介绍

- [x] 多优先级线程调度
- [x] 同优先级下的时间片轮转
- [x] 定时器
- [x] 空闲线程
- [x] shell
- [x] 信号量
- [x] 互斥量

​	在V1.3版本暂时还不支持线程间通信等基础功能，在近期版本更新后会补充完整。

​	在V1.3版本暂时不支持内存管理，所有的资源均为静态分配，在未来会补充完整。

## 快速上手


​	在使用V1.3版本时一定要拉取Releases版本的工程，请不要拉去main分支的工程，在最新的main分支里可能会存在未实现完的功能或者bug。


​	在安装好KEIL相关环境后，打开./USER/Template.uvprojx工程文件，将工程编译烧录进开发板（测试使用的是STM32F103C8T6，如果想用其他开发板测试记得改芯片型号和启动文件，只要是m3内核就可以），再将两个led灯的正极连接开发板的PB7 、PB8，负极接开发板的地，如果一个灯的闪烁频率为1s/次，另一个为0.5s/次，就可以正常使用了。

​	在测试例程中只有基础线程，如果想增加线程：

```
struct ky_thread ky_test3_thread; //声明线程句柄

ky_uint8_t ky_test3_thread_stack[512]; //开辟线程栈空间

void test3_thread_entry()
{
	while(1)
	{
		//线程任务
		//记得执行ky_thread_delay_s/ky_thread_delay_ms,否则无法正常调度
	}
}

int main()
{
	ky_thread_init(&ky_test3_thread,       			 //线程句柄
					"test3",						 //线程名称
					test3_thread_entry,				 //线程入口函数
					KY_NULL,						 //线程参数
					&ky_test3_thread_stack[0],		 //线程栈起始地址
					sizeof(ky_test3_thread_stack),	 //线程栈大小
					4,								 //线程优先级，这个值越大线程优先级越高(最高等级32，可以在kyconfig里修改)
					2);								 //时间片（有同优先级的线程才会生效）
					
	ky_thread_startup(&ky_test3 _thread);             //启动线程
	//在系统调度启动(ky_system_schedule_start)之前添加线程初始化和线程启动

}


```

## 更改配置

​	在./ky-thread/include下有kyconfig文件，如果想要更改系统配置，可以自行在文件中修改

## 关于本项目

​	如果你也对操作系统的实现感兴趣或者你对本项目有好的想法，想参与到本项目的开发中来，请发送邮件到1239625475@qq.com。

​	如果你在本项目中发现bug或者有可以改进的地方，欢迎提交PR。

​	在V1.4版本会实现线程间通信，敬请期待。

