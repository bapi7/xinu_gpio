/* subscribe.c - subscribe to a topic */

#include <xinu.h>

/*-----------------------------------------------------------------------------------
 *  subscribe  -  subscribe to a topic by providing a topicID and a callback function
 *-----------------------------------------------------------------------------------
 */

syscall subscribe(
          topic16       topic,                          /* ID of the intended topic     */
          void          (*handler)(topic16, void*, uint32)     /* Message List                 */
        )
{
        intmask mask;                   /* Saved interrupt mask         */
        struct  topicentry *topic_ptr;   /* Ptr to topic table entry */

        mask = disable();

	int curr_topicid = TOPIC_NUM(topic);
        int curr_groupid = GROUP_NUM(topic);

        if(handler == NULL) {
                kprintf("Not a valid call back funtion \n");
                return SYSERR;
        }

        topic_ptr = &topictab[topic];

        if(topic_ptr->nsubscribers == 8)
                return SYSERR;

        struct subscriber *temp = (struct subscriber*)(getmem(sizeof(struct subscriber)));
        temp->pr_id = currpid;
	temp->topicid = curr_topicid;
        temp->groupid = curr_groupid;
        temp->callback = handler;

        // add entry in the list
        temp->next = topic_ptr->subscriber_list;
        topic_ptr->subscriber_list = temp;
        topic_ptr->nsubscribers++;

        kprintf("Subscribed process %d on group %d to topic %d \n",currpid,curr_groupid,curr_topicid);
	restore(mask);
        return OK;
}
