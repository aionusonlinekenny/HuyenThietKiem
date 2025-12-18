#ifndef DBTABLE_H
#define DBTABLE_H

#include "db.h"
#include "malloc.h"

#define MAX_INDEX			16					
#define MAX_TABLE_NAME		256

typedef int (*GetIndexFunc)(DB *, const DBT *, const DBT *, DBT *);

typedef struct ZCursor {
	bool bTravel;								
	DBC *dbcp;									
	int index;									
	char *key;
	int key_size;
	char *data;									
	int size;									
}tagZCursor;

class ZDBTable {
	DB *primary_db;												
	DB *index_db[MAX_INDEX];								
	GetIndexFunc get_index_funcs[MAX_INDEX];					
	bool is_index_unique[MAX_INDEX];							
	int index_number;										
	char table_name[MAX_TABLE_NAME];
protected:
	char env_path[MAX_TABLE_NAME];
	DB_ENV *dbenv;												//数据库环境
	ZCursor *_search(bool bKey, const char *key_ptr, int key_size, int index);		//搜索指定记录
	bool _next(bool bKey, ZCursor *cursor);															//下一个记录
public:
	bool bStop;
	ZDBTable(const char *path, const char *name);			//环境目录和数据表的名字
	virtual ~ZDBTable();
	
	int addIndex(GetIndexFunc func, bool isUnique = false);		//增加索引项
	bool open();												//打开数据表
	void close();												//关闭数据表
	bool commit();												//基于事务的提交，目前使用自动提交，不使用

//基本记录操作
	bool add(const char *key_ptr, int key_size, const char *data_ptr, int data_size);
	bool remove(const char *key_ptr, int key_size, int index = -1);

	void closeCursor(ZCursor *cursor) {
		if(!cursor) return;
		if(cursor->bTravel) {
			free(cursor->key);
		}
		free(cursor->data);

		delete cursor;
	}
	ZCursor *first() {											//遍历数据库，得到第一条记录	
		return _search(false, NULL, 0, -1);
	}
	ZCursor *search(const char *key_ptr, int key_size, int index = -1){
		return _search(false, key_ptr, key_size, index);		//搜索指定记录
	}
	bool next(ZCursor *cursor) {											//下一个记录
		return _next(false, cursor);
	}
	ZCursor *search_key(const char *key_ptr, int key_size, int index = -1) {	//搜索指定记录，返回主键值
		return _search(true, key_ptr, key_size, index);
	}
	bool next_key(ZCursor *cursor) {															//下一个记录，返回主键值
		return _next(true, cursor);
	}
//下面是一些维护性的操作
	void deadlock() {						//解除死锁
		dbenv->lock_detect(dbenv, 0, DB_LOCK_DEFAULT, NULL);
	}
	void removeLog();						//清除日志文件
};

#define MAX_RETRY	16

#endif
