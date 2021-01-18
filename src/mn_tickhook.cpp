#if ( configUSE_TICK_HOOK == 1 )

#include "mn_tickhook.hpp"
#include "mn_micros.hpp"

///TODO: Add Critical Section

std::list<base_tickhook_entry*> base_tickhook::m_listHooks;
unsigned int ___current_ticks_base_tickhook_dd789 = 0;


int base_tickhook::register(base_tickhook_entry* entry) {
    int ret = NO_ERROR;

    for (list<base_tickhook_entry *>::iterator it = base_tickhook::m_listHooks.begin();
         it != base_tickhook::m_listHooks.end();
         ++it) {

        base_tickhook_entry *_entry = *it;

        if(entry == _entry) {
            ret = ERR_TICKHOOK_ADD;
            break;
        }
    }
    if(ret != ERR_TICKHOOK_ADD) {
        m_listHooks.push_front(entry);
    }
    return ret;
}

void vApplicationTickHook(void) {
    ___current_ticks_base_tickhook_dd789++;

   for (list<base_tickhook_entry *>::iterator it = base_tickhook::m_listHooks.begin();
         it != base_tickhook::m_listHooks.end();
         ++it) {

        base_tickhook_entry *entry = *it;

        if(!entry->is_ready()) continue;

        if( (___current_ticks_base_tickhook_dd789 % entry->get_ticks()) == 0) {
            entry->on_tick();

            if(entry->is_oneshoted()) {
                m_listHooks.remove(entry);
            }
        }
    }
}

#endif