import { useEffect, useState } from 'react';
import { Spinner, Table } from 'react-bootstrap';
import { useHistory } from 'react-router-dom';
import { DbProject } from './DatabaseInterface';
import axios from 'axios';
import { BackButton } from './ui/BackButton';
import { CopyButton } from './ui/CopyButton';
import { RemoveButton } from './ui/RemoveButton';
import { OpenButton } from './ui/OpenButton';

type ProjectListItemProps = {
    id: number;
    name: string;
    title: string;
    onDeleteItem: (id: number) => void;
}

function ProjectListItem(props: ProjectListItemProps) {
    const history = useHistory();

    const onOpen = () => { history.push("/project/" + props.id); }
    const onCopy = () => {
        console.log("click on open icon")
    }
    const onDelete = () => {
        props.onDeleteItem(props.id);
    }

    return (
        <tr>
            <td><OpenButton onClick={onOpen} disabled={true} /></td>
            <td>{props.name}</td>
            <td>{props.title}</td>
            <td>"last modifed"</td>
            <td><CopyButton onClick={onCopy} disabled={false} /></td>
            <td><RemoveButton onClick={onDelete} disabled={false} /></td>
        </tr>
    )
}

export function ProjectList() {

    const [result, setResult] = useState<DbProject[]>([])
    const [loading, setLoading] = useState(true)

    const onDelete = (id: number) => {
        console.log("on delete: ", id);
    };

    async function getData() {
        await axios.get(process.env.BACKEND + '/projects')
            .then(response => setResult(response.data))
            .finally(() => setLoading(false))
    }

    useEffect(() => { getData() }, []);

    if (loading)
        return (<Spinner animation="border" role="status"></Spinner>);

    return result ? <>
        <BackButton onClick={() => { }} disabled={true} />
        <Table className="projectlist" borderless hover size="sm">
            <thead>
                <tr>
                    <th></th>
                    <th>Name</th>
                    <th>Description</th>
                    <th>Last modifed</th>
                </tr>
            </thead>
            <tbody>
                {result.map(e => (<ProjectListItem id={e.id} name={e.name} title={e.title} onDeleteItem={onDelete} />))}
            </tbody>
        </Table>
    </> :
        <>
            error
        </>;
}
