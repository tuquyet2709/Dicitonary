#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"bt/inc/btree.h"
//#include"thuvien.h
#include<gtk/gtk.h>

FILE *f;
BTA *book;
char *anh;
char *viet;
GtkTextBuffer *tb;
GtkTextBuffer *tb1;
GtkTextBuffer *tb2;
GtkTextBuffer *tb3;

void docfile(FILE *f,BTA *book)
{
	char english[20];
	char vietnam[10000];
	char read[100];
	char tmp;
	int i=0,k=0;
	
	btinit();
	book=btopn("td.txt",0,0);
	if(!book) {
		book=btcrt("td.txt",0,0);

		f=fopen("anhviet.txt","r");
		if (f==NULL) printf("Khong mo duoc file\n");
	
		while (fgets(read,100,f)!=NULL)	{
			if(read[0]=='@') {
				if(strlen(vietnam)!=0)	btins(book,english,vietnam,sizeof(vietnam));
				k=1;
				for(i=0;i<strlen(read);i++)	{
					if(read[i+1]=='/') english[i]=0;
					else english[i]=read[i+1];
				}
				english[strlen(english)-1]=0;
			}
			else {
				if(k==1) {	
					strcpy(vietnam,read);
					k=0;
				}
				else strcat(vietnam,read);
			}
		}
		btins(book,english,vietnam,sizeof(vietnam));
		fclose(f);
	}
	//printf("ok\n");
	btcls(book);
}

char *timkiem(char *key)
{
	char data[10000];
	char *data1;
	book=btopn("td.txt",0,0);
	int size,i;
	i=btsel(book,key,data,sizeof(data),&size);
	if(i!=0) data1="Từ này chưa có trong từ điển.\n";
	else data1=(char*)data;
	btcls(book);
	return data1;
}

void tratu(GtkWidget *widget,gpointer label)
{
	GtkTextIter start;
	GtkTextIter end;
	
	gtk_text_buffer_get_start_iter (tb, &start);
	gtk_text_buffer_get_end_iter (tb, &end);
	anh=gtk_text_buffer_get_text (tb, &start, &end, FALSE);
	viet=timkiem(anh);
	gtk_label_set_text(label,viet);
}

void add()
{
	GtkTextIter start;
	GtkTextIter end;
	
	gtk_text_buffer_get_start_iter (tb1, &start);
	gtk_text_buffer_get_end_iter (tb1, &end);
	anh=gtk_text_buffer_get_text (tb1, &start, &end, FALSE);
	
	gtk_text_buffer_get_start_iter (tb2, &start);
	gtk_text_buffer_get_end_iter (tb2, &end);
	viet=gtk_text_buffer_get_text (tb2, &start, &end, FALSE);
	
	book=btopn("td.txt",0,0);
	btins(book,anh,viet,10000);
	//printf("Anh:%s\nViet:%s\n",anh,viet);
	char data[10000];
	int size,i;
	i=btsel(book,anh,data,sizeof(data),&size);
	//if(i==0) printf("Thanh cong: data %s\n",data);
	
	btcls(book);
}

void them(int argc,char *argv[])
{
	GtkWidget *w;
	GtkWidget *fix;
	GtkWidget *entry1;
	GtkWidget *entry2;
	GtkWidget *label1;
	GtkWidget *label2;
	GtkWidget *ok;
		
	gtk_init(&argc,&argv);
	w=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(w),GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(w),"Bổ sung từ mới");
	gtk_window_set_default_size(GTK_WINDOW(w),250,400);
	g_signal_connect(w,"destroy",gtk_main_quit,NULL);
		
	fix=gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(w),fix);
	
	label1=gtk_label_new("Nhập từ Tiếng Anh:");
	gtk_label_set_justify(GTK_LABEL(label1),GTK_JUSTIFY_LEFT);
	gtk_fixed_put(GTK_FIXED(fix),label1,10,30);
	
	label2=gtk_label_new("Nhập nghĩa Tiếng Việt:");
	gtk_label_set_justify(GTK_LABEL(label2),GTK_JUSTIFY_LEFT);
	gtk_fixed_put(GTK_FIXED(fix),label2,10,90);
	
	entry1=gtk_text_view_new();
	gtk_widget_set_size_request(entry1,200,30);
	gtk_fixed_put(GTK_FIXED(fix),entry1,10,50);
	tb1=gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry1));
	
	entry2=gtk_text_view_new();
	gtk_widget_set_size_request(entry2,200,220);
	gtk_fixed_put(GTK_FIXED(fix),entry2,10,110);
	tb2=gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry2));

	ok=gtk_button_new_with_label("OK");
	gtk_widget_set_size_request(ok,100,30);
	gtk_fixed_put(GTK_FIXED(fix),ok,75,360);
	g_signal_connect(ok,"clicked",G_CALLBACK(add),w);
	
	gtk_widget_show_all(w);
	gtk_main();
}

void delete()
{
	GtkTextIter start;
	GtkTextIter end;
	
	gtk_text_buffer_get_start_iter (tb3, &start);
	gtk_text_buffer_get_end_iter (tb3, &end);
	anh=gtk_text_buffer_get_text (tb3, &start, &end, FALSE);

	book=btopn("td.txt",0,0);
	btdel(book,anh); //tra ve 0 neu xoa thanh cong
	btcls(book);
}

void xoa(int argc,char *argv[])
{
	GtkWidget *w;
	GtkWidget *entry3;
	GtkWidget *label;
	GtkWidget *fix;
	GtkWidget *button;
	
	gtk_init(&argc,&argv);
	w=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(w),GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(w),"Xóa từ");
	gtk_window_set_default_size(GTK_WINDOW(w),250,400);
	g_signal_connect(w,"destroy",gtk_main_quit,NULL);
		
	fix=gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(w),fix);
	
	label=gtk_label_new("Nhập từ muốn xóa:");
	gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
	gtk_fixed_put(GTK_FIXED(fix),label,10,30);
	
	entry3=gtk_text_view_new();
	gtk_widget_set_size_request(entry3,200,30);
	gtk_fixed_put(GTK_FIXED(fix),entry3,10,50);
	tb3=gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry3));
	
	button=gtk_button_new_with_label("OK");
	gtk_widget_set_size_request(button,100,30);
	gtk_fixed_put(GTK_FIXED(fix),button,75,360);
	g_signal_connect(button,"clicked",G_CALLBACK(delete),w);
	
	gtk_widget_show_all(w);
	gtk_main();
}

int main(int argc,char *argv[])
{
	docfile(f,book);
	
	GtkWidget *window;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *button3;
	GtkWidget *fix;
	GtkWidget *textview;
	GtkWidget *label;
	
	gtk_init(&argc,&argv);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window),"TỪ ĐIỂN ANH-VIỆT");
	gtk_window_set_default_size(GTK_WINDOW(window),650,500);
	g_signal_connect(window,"destroy",gtk_main_quit,NULL);
	
	fix=gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window),fix);
	
	button1=gtk_button_new_with_label("Tra từ");
	gtk_widget_set_size_request(button1,120,40);
	gtk_fixed_put(GTK_FIXED(fix),button1,250,30);
	
	button2=gtk_button_new_with_label("Bổ sung từ mới");
	gtk_widget_set_size_request(button2,120,40);
	gtk_fixed_put(GTK_FIXED(fix),button2,370,30);
	
	button3=gtk_button_new_with_label("Xóa từ");
	gtk_widget_set_size_request(button3,120,40);
	gtk_fixed_put(GTK_FIXED(fix),button3,490,30);
	
	textview=gtk_text_view_new();
	gtk_widget_set_size_request(textview,200,20);
	gtk_fixed_put(GTK_FIXED(fix),textview,25,35);
	tb=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

	label=gtk_label_new("");
	gtk_label_set_justify(GTK_LABEL (label), GTK_JUSTIFY_LEFT);
	//gtk_label_set_max_width_chars ( (GtkLabel*)label,20); //co dinh chieu dai max cua label- chua thanh cong
	gtk_fixed_put(GTK_FIXED(fix),label,250,100);
	
	g_signal_connect(button1,"clicked",G_CALLBACK(tratu),label);
	g_signal_connect(button2,"clicked",G_CALLBACK(them),NULL);
	g_signal_connect(button3,"clicked",G_CALLBACK(xoa),NULL);

	GtkWidget *tree;
	tree=gtk_tree_view_new();
	
	
	gtk_widget_show_all(window);
	gtk_main();
}
