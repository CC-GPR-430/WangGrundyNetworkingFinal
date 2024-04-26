
template <typename T>
size_t copy_to_buffer(char* buffer, T* object, size_t buffer_size)
{
    if (sizeof(T) > buffer_size) return 0;
    char* object_as_bytes = (char*)object;
    for (int i = 0; i < sizeof(T); i++) {
        buffer[i] = object_as_bytes[i];
    }
    return sizeof(T);
}

template <typename T>
size_t copy_from_buffer(const char* buffer, T* object, size_t buffer_size)
{
    if (sizeof(T) > buffer_size) return 0;
    char* object_as_bytes = (char*)object;
    for (int i = 0; i < sizeof(T); i++) {
        object_as_bytes[i] = buffer[i];
    }
    return sizeof(T);
}

size_t copy_to_buffer(char* buffer, int* object, size_t buffer_size)
{
    if (buffer_size < sizeof(int)) return 0;
    char* object_as_bytes = (char*)object;
    for (int i = 0; i < sizeof(int); i++) {
        buffer[i] = object_as_bytes[i];
    }
    return sizeof(int);
}

// Second option: Serialize as binary
size_t SerializeGameObjectAsBytes(const GameObject* go, char* buffer, size_t buffer_size)
{
    size_t bytes_written = 0;
    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->x, buffer_size - bytes_written);
    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->y, buffer_size - bytes_written);
    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->z, buffer_size - bytes_written);
    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->xVel, buffer_size - bytes_written);
    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->yVel, buffer_size - bytes_written);
    bytes_written += copy_to_buffer(&buffer[bytes_written], &go->zVel, buffer_size - bytes_written);
    return bytes_written;
}

size_t DeserializeGameObjectAsBytes(GameObject* go, char* buffer, size_t buffer_size)
{
    size_t bytes_read = 0;

    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->x, buffer_size - bytes_read);
    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->y, buffer_size - bytes_read);
    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->z, buffer_size - bytes_read);
    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->xVel, buffer_size - bytes_read);
    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->yVel, buffer_size - bytes_read);
    bytes_read += copy_from_buffer(&buffer[bytes_read], &go->zVel, buffer_size - bytes_read);

    return bytes_read;
}