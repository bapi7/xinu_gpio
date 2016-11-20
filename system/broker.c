/* broker.c*/

#include <xinu.h>

process broker() {

        while(1) {
                // fetch head from the pubqueue

                if(pqueue.count) {
                        // Get current entry from pubentry
                        struct pubentry *curr = pqueue.head;
                        pqueue.head = pqueue.head->next;
                        pqueue.count--;

                        int curr_topicid = TOPIC_NUM(curr->topicid);
                        int curr_groupid = GROUP_NUM(curr->topicid);
			struct topicentry *topic_ptr = &topictab[curr->topicid];

                        // No subscribers on the topic
                        if(topic_ptr->nsubscribers == 0) {
                                kprintf("No subscribers to the group %d topic %d \n",curr_groupid,curr_topicid);
				return OK;
                        }

                        struct subscriber *temp = topic_ptr->subscriber_list;

                        // Invoke callback function for each of the topic subscribers in the list.
                        while(temp) {
				if(temp->groupid == curr_groupid || curr_groupid == 0) {
	                                temp->callback(curr->topicid,curr->data,curr->nbytes);
				}
                                temp = temp->next;
                        }
			freemem((char *)curr,sizeof(struct pubentry));
                } else {
			sleep(2);
		}
        }

}
