import React, { useEffect, useState } from "react";
import { Spinner, Table, Alert } from "react-bootstrap";
import { RouteComponentProps, useHistory } from "react-router-dom";
import { CopyButton } from "./ui/CopyButton";
import { DbCrossSection } from "./DatabaseInterface";
import { EditButton } from "./ui/EditButton";
import { RemoveButton } from "./ui/RemoveButton";
import axios from 'axios';

function CrossSectionListItem(props: DbCrossSection) {
    const history = useHistory();

    const onEdit = () => { history.push("/crosssection/" + props.id); }
    const onCopy = () => {
    }
    const onDelete = () => {
    }
    return (
        <tr>
            <td><EditButton onClick={onEdit} disabled={false}></EditButton></td>
            <td>{props.name}</td>
            <td>{props.area}</td>
            <td><CopyButton onClick={onCopy} disabled={false} /></td>
            <td><RemoveButton onClick={onDelete} disabled={false} /></td>
        </tr>
    )
}

export type CrossSectionListProps = {
    projectId: string;
}

export function CrossSectionList(props: CrossSectionListProps) {

    const [data, setData] = useState<DbCrossSection[]>([])
    const [loading, setLoading] = useState(true)

    async function getData() {
        await axios.get(process.env.BACKEND + '/crosssections/' + props.projectId)
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
        return (<Alert variant='danger'>No cross sections defined.</Alert>);
    }

    return (
        <>
            <Table borderless hover size="sm">
                <thead>
                    <tr>
                        <th></th>
                        <th>Name</th>
                        <th>Area</th>
                    </tr>
                </thead>
                <tbody>
                    {data.map(e => (<CrossSectionListItem id={e.id} name={e.name} uuid={e.uuid} area={e.area} />))}
                </tbody>
            </Table>
        </>
    )
}

export interface RouterComponentCrossSectionListProps extends RouteComponentProps<CrossSectionListProps> { }
