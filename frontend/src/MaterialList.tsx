import { useEffect, useState } from "react";
import { Alert, Spinner, Table } from "react-bootstrap";
import { CopyButton } from "./ui/CopyButton";
import { DbMaterial } from "./DatabaseInterface";
import { EditButton } from "./ui/EditButton";
import { RemoveButton } from "./ui/RemoveButton";
import axios from 'axios';

function MaterialListItem(props: DbMaterial) {

    const onEdit = () => { }
    const onCopy = () => {
    }
    const onDelete = () => {
    }
    return (
        <tr>
            <td><EditButton onClick={onEdit} disabled={false}></EditButton></td>
            <td>{props.name}</td>
            <td>{props.e_modules}</td>
            <td><CopyButton onClick={onCopy} disabled={false} /></td>
            <td><RemoveButton onClick={onDelete} disabled={false} /></td>
        </tr>
    )
}

export type MaterialListProps = {
    projectId: string;
}

export function MaterialList(props: MaterialListProps) {

    const [data, setData] = useState<DbMaterial[]>([])
    const [loading, setLoading] = useState(true)

    async function getData() {
        await axios.get(process.env.BACKEND + '/materials/' + props.projectId)
            .then(response => setData(response.data))
            .finally(() => setLoading(false))
    }

    useEffect(() => { getData() }, []);

    const onDelete = (id: number) => {
        console.log("on delete: ", id);
    };

    if (loading) {
        return (<><Spinner animation="border" role="status"></Spinner></>);
    }

    if (data.length === 0) {
        return (<Alert variant='danger'>No material defined.</Alert>);
    }

    return (
        <>
            <Table borderless hover size="sm">
                <thead>
                    <tr>
                        <th></th>
                        <th>Name</th>
                        <th>E modules</th>
                    </tr>
                </thead>
                <tbody>
                {data.map(e => (
                        <MaterialListItem
                            id={e.id}
                            name={e.name}
                            uuid={e.uuid}
                            e_modules={e.e_modules}
                        />))}
                </tbody>
            </Table>
        </>
    )
}
