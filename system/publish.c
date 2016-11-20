/* publish.c - publish data to a topic */

#include <xinu.h>

/*-------------------------------------
 *  publish  -  publish data to a topic
 *-------------------------------------
 */

syscall publish(topic16 topic,void *data,uint32 nbytes) {

	struct pubentry *temp = (struct pubentry*)getmem(sizeof(struct pubentry));
	temp->topicid = topic;
	temp->data = data;
	temp->nbytes = nbytes;
	temp->next = NULL;
	
	// if list is empty
	if(pqueue.head == NULL)
	{
		pqueue.head = temp;
		pqueue.tail = temp;
		pqueue.count++;
		return 0;
	}

	pqueue.tail->next = temp;
	pqueue.tail = temp;
	pqueue.count++;
	
	kprintf("Published %d bytes of data on group %d topic %d \n",nbytes,GROUP_NUM(topic),TOPIC_NUM(topic));
	
	return 0;
}
