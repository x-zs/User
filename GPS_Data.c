#include "GPS_Data.h"
void gpsdata(uint8_t *str,uint8_t *ptr)//str指向串口接收缓冲区，ptr指向目标缓冲区。
{
	uint32_t i=0,j=0,flag=0,douhao=0;
	uint8_t gps[6]="$GPRMC",end[4]="\r\n";
	while(str[i])
	{
		 if(str[i]==gps[j])   //查找'$'
	 {
		  flag=1;
		  i++;
		  j++;
	    while(flag&&j<6)              //查找$GPRMC
	   {
		   if(str[i]==gps[j])
		  {
			  i++;
			  j++;
		  }
		   else 
			 flag=0;
	   }
	    while(str[i]&&douhao<3&&flag==1)//一直查找到纬度
	   {
		  if(str[i]==',')
		 {
			douhao++;
			i++;
		 }
		  else i++;
	  }
		 j=0;
	    while(str[i]!=','&&flag==1)//截取纬度
	  {
		  
	    ptr[j]=str[i];
			j++;
			i++;
	  }
	    ptr[j++]=' ';
	    i=i+3;
	    while(str[i]!=','&&flag==1)//截取经度
	  {
	    ptr[j++]=str[i++];
	  }
	   i=0;
	   while(i<4&&flag==1)
		 {
		  ptr[j]=end[i];
		  j++;
		  i++;
	   }
	   ptr[j]='\0';
   }
	else i++;
}
	}

void GPS_Dispose(uint8_t *str,uint8_t *ptr1,uint8_t *ptr2)    //将字符型GPS数据进一步处理，然后再转化为字符型输出。
{
	uint32_t i=0,j=0,k=0,m,n,mid1,mid2,Beishu=10,flag;
	uint8_t flag1,end[4]="\r\n";
	float num1=0,num11=0,num2=0,num22=0,sum1=0,sum2=0;
	
	while(str[i])            //将经纬度中分数分离出来，分别放入ptr1和ptr2字符数组中。
	{
		if(i>=2&&i<10)
		{
			ptr1[j]=str[i];
			i++;
			j++;
		}
		else if(i>=14&&i<22)
		{
			ptr2[k]=str[i];
			i++;
			k++;
		}
		else i++;
	}
	ptr1[j]='\0';
	ptr2[k]='\0';

	i=0;
	while(ptr1[i])                      //将纬度中分数转化为浮点型，
	{
		if(ptr1[i]=='.')
		{
			flag=i;
			i=0;
			while(i<flag)
			{
				num1=num1*10.0+ptr1[i]-'0';
				i++;
			}
			i=flag+5;
			while(i>flag)
			{
				num11=num11/10.0+ptr1[i]-'0';
				i--;
			}
			if(i==flag)
				num11=num11/10.0;
      i++;
		}
		else i++;
			
	}
	num1=num1+num11;     //加起来就是转化后的浮点型分数值
	num1=num1/60.0;      //分数值要除以60转化为度。
	i=0;
	while(ptr2[i])        //将截取的经度的分数转化为浮点型
	{
		if(ptr2[i]=='.')
		{
			flag=i;
			i=0;
			while(i<flag)
			{
				num2=num2*10+ptr2[i]-'0';
				i++;
			}
			i=flag+5;
			while(i>flag)
			{
				num22=num22/10.0+ptr2[i]-'0';
				i--;
			}
			if(i==flag)
				num22=num22/10.0;
				i++;
		}
		else i++;
	}
	num2=num2+num22;               
	num2=num2/60.0;              //纬度分数值转化为度。
	
	i=0;
	while(i<2)                    //经纬度中未处理的纬度的度值转化为浮点型。
	{
		sum1=sum1*10+str[i]-'0';
		i++;
	}
	i=11;
	while(i<14)                   //经度中未处理的度值转化为浮点型。
	{
		sum2=sum2*10+str[i]-'0';
		i++;
	}
	sum1=sum1+num1;                //处理成功的纬度值
	sum2=sum2+num2;                //处理成功的经度值
	
	mid1=(int)sum1;
	mid2=(int)sum2;
	
	
	i=0;
	ptr1[i]=mid1/10+'0';         //纬度中整数部分转化为字符型。
	i++;
	ptr1[i]=mid1%10+'0';
	i++;
	while(sum1>1.0)
	--sum1;
	
	
	m=0;
	while(m<5)                       //对纬度中小数部分进行字符转化
	{
		flag=(int)(sum1*Beishu);
	  flag=flag%10;
		ptr1[i]=flag+'0';
		m++;
		i++;
		Beishu=Beishu*10;
	}
	ptr1[i]='\0';
	
	
	i=0;
	Beishu=10;
	ptr2[i]=mid2/100+'0';
	i++;
	ptr2[i]=(mid2%100)/10+'0';
	i++;
	ptr2[i]=mid2%10+'0';
	i++;
	
	
	while(sum2>1.0)
	--sum2;
	m=0;
	Beishu=10;
	while(m<5)                         //对经度中小数部分进行转化
	{
		flag=(int)(sum2*Beishu);
		flag=flag%10;
		ptr2[i]=flag+'0';
		m++;
		i++;
		Beishu=Beishu*10;
	}
	ptr2[i]='\0';
	
	i=0;
	j=2;
	str[0]='%';
	str[1]='%';
	while(ptr1[i])                    //将字符型纬度重新存入str数组中.
	{
		str[j]=ptr1[i];
		i++;
		j++;
	}
	str[j]=',';
	j++;
	i=0;
	
	while(ptr2[i])                    //将字符型经度重新存入str数组中。
	{
		str[j]=ptr2[i];
		i++;
		j++;
	}
	while(str[j])
	{
		str[j]=0;
		j++;
	}
	str[j]='\0';
	i=0;
	while(str[i])
		++i;
	while(i>=4)
	{
		str[i+1]=str[i];
		--i;
	}
	i++;
	str[i]='.';
	
	i=0;
	while(str[i]!=',')
	   ++i;
	i=i+4;
	j=0;
	while(str[j])
		++j;
	while(j>=i)
	{
		str[j+1]=str[j];
		--j;
	}
	j++;
	str[j]='.';
	j=j+6;
	str[j]='%';
	j++;
	str[j]='%';
	i=0;
	while(str[i])
		++i;
	j=0;
	while(j<4)
	{
		str[i]=end[j];
		i++;
		j++;
	}
  str[i]='\0';
}
