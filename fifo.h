#ifndef FIFO_H_
#define FIFO_H_

#define FIFO_CREATE(name, size, type) \
		static volatile uint16_t name ## _insert_index; \
		static volatile uint16_t name ## _remove_index; \
		static type name ## _fifo[size]; \
		static inline void name ## _fifo_init() { \
			name ## _insert_index = name ## _remove_index = 0; \
		} \
		static inline uint8_t name ## _fifo_insert(type data) { \
			if((name ## _insert_index - name ## _remove_index) & ~(size-1)){ \
				return 0; \
			} \
			name ## _fifo[name ## _insert_index & (size-1)] = data; \
			name ## _insert_index++; \
			return 1; \
		} \
		static inline uint8_t name ## _fifo_remove(type *data_ptr) { \
			if(name ## _insert_index == name ## _remove_index) { \
				return 0; \
			} \
			*data_ptr = name ## _fifo[name ## _remove_index & (size-1)]; \
			name ## _remove_index++; \
			return 1; \
		} \
		uint16_t name ## _fifo_size() { \
			return name ## _remove_index - name ## _insert_index; \
		}



#endif /* FIFO_H_ */
