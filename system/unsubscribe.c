/* unsubscribe.c - unsubscribe a process from a topic */

#include <xinu.h>

/*-------------------------------------------------------
 *  unsubscribe  -  unsubscribe from a topic
 *-------------------------------------------------------
 */
syscall unsubscribe (topic16 topic) {

        intmask mask;                   /* Saved interrupt mask         */
        struct  topicentry *topic_ptr;   /* Ptr to topic table entry */

        mask = disable();

        int curr_topicid = TOPIC_NUM(topic);
        int curr_groupid = GROUP_NUM(topic);

	topic_ptr = &topictab[topic];

        if(topic_ptr->nsubscribers == 0) {
                kprintf("No subscriber to unsubscribe on topic %d \n",topic);
                return SYSERR;
        }


        struct subscriber *curr = topic_ptr->subscriber_list;
        struct subscriber *prev = NULL;

        while(curr != NULL) {
                if(curr->pr_id == currpid && curr->topicid == curr_topicid && curr->groupid == curr_groupid) {
                        kprintf("Unsubscribed process %d from group %d topic %d \n",currpid,curr_topicid,curr_groupid);
                        // if head node
                        if(prev == NULL)
                                topic_ptr->subscriber_list = curr->next;

                        // if not head node
                        else
                                prev->next = curr->next;

                        freemem((char *)curr,sizeof(struct subscriber));
                        topic_ptr->nsubscribers--;
                        return OK;
                }
                prev = curr;
                curr = curr->next;
        }
	restore(mask);
        return SYSERR;
}
