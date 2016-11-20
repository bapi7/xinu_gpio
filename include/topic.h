/* topic.h */

/* Maximum number of topics */

#ifndef TOPIC_SIZE
#define TOPIC_SIZE      256
#endif

#define TOPIC_NUM(x) ( (x & 0x00FF) )
#define GROUP_NUM(x) ( ((x & 0xFF00) >> 8) )

struct subscriber {
        pid32 pr_id;
	topic16 topicid;
	topic16 groupid;
        void  (*callback)(topic16, void*, uint32);
        struct subscriber *next;
};

struct topicentry {
        struct subscriber *subscriber_list;
        int nsubscribers;
};

struct topicentry topictab[TOPIC_SIZE];

struct pubentry {
	topic16 topicid;
	void *data;
	uint32 nbytes;
	struct pubentry *next;
};

struct pubqueue {
	uint32 count;
	struct pubentry *head;
	struct pubentry *tail;
};

struct pubqueue pqueue;

process broker();
