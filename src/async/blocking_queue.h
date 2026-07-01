#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

/**
 * @file blocking_queue.h
 * @brief Потокобезопасная блокирующая очередь.
 */

#include <condition_variable>
#include <deque>
#include <mutex>
#include <utility>

/**
 * @brief Очередь для передачи задач между потоками.
 *
 * Использует std::deque как контейнер хранения и добавляет синхронизацию для
 * producer/consumer-сценариев. После close() новые элементы не принимаются, а
 * pop() возвращает false, когда очередь опустела.
 */
template <typename T>
class BlockingQueue {
public:
    /**
     * @brief Добавляет элемент в конец очереди.
     *
     * @return true, если элемент добавлен; false, если очередь закрыта.
     */
    bool push(const T& value) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_closed) {
                return false;
            }
            m_items.push_back(value);
        }
        m_condition.notify_one();
        return true;
    }

    /**
     * @brief Добавляет элемент в конец очереди перемещением.
     *
     * @return true, если элемент добавлен; false, если очередь закрыта.
     */
    bool push(T&& value) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_closed) {
                return false;
            }
            m_items.push_back(std::move(value));
        }
        m_condition.notify_one();
        return true;
    }

    /**
     * @brief Извлекает следующий элемент.
     *
     * Блокируется, пока в очереди не появится элемент или пока очередь не будет
     * закрыта. Если очередь закрыта и пуста, возвращает false.
     */
    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this]() { return m_closed || !m_items.empty(); });

        if (m_items.empty()) {
            return false;
        }

        value = std::move(m_items.front());
        m_items.pop_front();
        return true;
    }

    /**
     * @brief Закрывает очередь и будит ожидающие pop().
     */
    void close() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_closed = true;
        }
        m_condition.notify_all();
    }

    /**
     * @brief Проверяет, закрыта ли очередь.
     */
    bool is_closed() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_closed;
    }

private:
    /// Контейнер элементов очереди.
    std::deque<T> m_items;

    /// Mutex для доступа к контейнеру и флагу закрытия.
    mutable std::mutex m_mutex;

    /// Условная переменная для ожидания элементов или закрытия.
    std::condition_variable m_condition;

    /// Признак закрытой очереди.
    bool m_closed = false;
};

#endif  // BLOCKING_QUEUE_H
