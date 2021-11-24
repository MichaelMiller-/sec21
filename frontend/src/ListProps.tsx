export interface ListProps<T> {
   projectId: string
   items: Array<T>
   onUpdate: () => void
   onDelete: (id: number) => void
}
